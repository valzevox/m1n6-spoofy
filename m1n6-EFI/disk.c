/*
 * m1n6-EFI / disk.c
 * Language: C (UEFI EDK2 Standard)
 * Target: NVMe & SATA Disk Serial Spoofing in Pre-Boot Environment
 */

#include "disk.h"
#include "smbios.h"

#define MAX_HOOKED_DISKS 32

typedef struct {
    EFI_DISK_INFO_PROTOCOL     *DiskInfo;
    EFI_DISK_INFO_IDENTIFY_DATA OriginalIdentify;
    EFI_DISK_INFO_INQUIRY       OriginalInquiry;
    BOOLEAN                     IsNvme;
} HOOKED_DISK_ENTRY;

STATIC HOOKED_DISK_ENTRY gHookedDisks[MAX_HOOKED_DISKS];
STATIC UINTN             gHookedDiskCount = 0;

STATIC
VOID
M1n6SpoofIdentifyBuffer(
    IN OUT UINT8 *Buffer,
    IN UINT32 BufferSize,
    IN BOOLEAN IsNvme
)
{
    CONST CHAR8 AlphaNum[] = "0123456789ABCDEF";
    UINTN i;

    if (Buffer == NULL || BufferSize == 0) {
        return;
    }

    if (IsNvme) {
        /* NVMe Controller Identify: Serial Number at offset 4..23 (20 bytes) */
        if (BufferSize >= (NVME_SN_OFFSET + NVME_SN_LEN)) {
            for (i = 0; i < NVME_SN_LEN; i++) {
                Buffer[NVME_SN_OFFSET + i] = AlphaNum[M1n6Random32() % (sizeof(AlphaNum) - 1)];
            }
        }
    } else {
        /* ATA/IDE Identify: Serial Number at offset 20..39 (20 bytes, byte-swapped pairs) */
        if (BufferSize >= (ATA_SN_OFFSET + ATA_SN_LEN)) {
            for (i = 0; i < ATA_SN_LEN; i += 2) {
                CHAR8 c1 = AlphaNum[M1n6Random32() % (sizeof(AlphaNum) - 1)];
                CHAR8 c2 = AlphaNum[M1n6Random32() % (sizeof(AlphaNum) - 1)];
                Buffer[ATA_SN_OFFSET + i]     = c2;
                Buffer[ATA_SN_OFFSET + i + 1] = c1;
            }
        }
    }
}

STATIC
EFI_STATUS
EFIAPI
M1n6HookedIdentify(
    IN EFI_DISK_INFO_PROTOCOL *This,
    IN OUT VOID               *IdentifyData,
    IN OUT UINT32             *IdentifyDataSize
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN i;
    HOOKED_DISK_ENTRY *Target = NULL;

    for (i = 0; i < gHookedDiskCount; i++) {
        if (gHookedDisks[i].DiskInfo == This) {
            Target = &gHookedDisks[i];
            break;
        }
    }

    if (Target != NULL && Target->OriginalIdentify != NULL) {
        Status = Target->OriginalIdentify(This, IdentifyData, IdentifyDataSize);
        if (!EFI_ERROR(Status) && IdentifyData != NULL && IdentifyDataSize != NULL) {
            M1n6SpoofIdentifyBuffer((UINT8*)IdentifyData, *IdentifyDataSize, Target->IsNvme);
        }
    }

    return Status;
}

STATIC
EFI_STATUS
EFIAPI
M1n6HookedInquiry(
    IN EFI_DISK_INFO_PROTOCOL *This,
    IN OUT VOID               *InquiryData,
    IN OUT UINT32             *InquiryDataSize
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN i;
    HOOKED_DISK_ENTRY *Target = NULL;

    for (i = 0; i < gHookedDiskCount; i++) {
        if (gHookedDisks[i].DiskInfo == This) {
            Target = &gHookedDisks[i];
            break;
        }
    }

    if (Target != NULL && Target->OriginalInquiry != NULL) {
        Status = Target->OriginalInquiry(This, InquiryData, InquiryDataSize);
        if (!EFI_ERROR(Status) && InquiryData != NULL && InquiryDataSize != NULL) {
            M1n6SpoofIdentifyBuffer((UINT8*)InquiryData, *InquiryDataSize, Target->IsNvme);
        }
    }

    return Status;
}

EFI_STATUS
M1n6HookDiskProtocols(VOID)
{
    EFI_STATUS Status;
    UINTN HandleCount = 0;
    EFI_HANDLE *HandleBuffer = NULL;
    UINTN i;
    EFI_DISK_INFO_PROTOCOL *DiskInfo;

    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiDiskInfoProtocolGuid,
        NULL,
        &HandleCount,
        &HandleBuffer
    );

    if (EFI_ERROR(Status) || HandleCount == 0) {
        Print(L"[m1n6-EFI] No DiskInfo protocol handles located\n");
        return Status;
    }

    for (i = 0; i < HandleCount && gHookedDiskCount < MAX_HOOKED_DISKS; i++) {
        Status = gBS->OpenProtocol(
            HandleBuffer[i],
            &gEfiDiskInfoProtocolGuid,
            (VOID**)&DiskInfo,
            gImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        );

        if (!EFI_ERROR(Status) && DiskInfo != NULL) {
            BOOLEAN isNvme = FALSE;
            
            /* Detect interface GUID (NVMe vs AHCI/IDE) */
            if (CompareGuid(&DiskInfo->Interface, &gEfiDiskInfoNvmeInterfaceGuid)) {
                isNvme = TRUE;
            }

            gHookedDisks[gHookedDiskCount].DiskInfo = DiskInfo;
            gHookedDisks[gHookedDiskCount].OriginalIdentify = DiskInfo->Identify;
            gHookedDisks[gHookedDiskCount].OriginalInquiry = DiskInfo->Inquiry;
            gHookedDisks[gHookedDiskCount].IsNvme = isNvme;

            DiskInfo->Identify = M1n6HookedIdentify;
            DiskInfo->Inquiry = M1n6HookedInquiry;

            gHookedDiskCount++;
        }
    }

    if (HandleBuffer != NULL) {
        gBS->FreePool(HandleBuffer);
    }

    Print(L"[m1n6-EFI] Successfully hooked %d storage controller devices\n", gHookedDiskCount);
    return EFI_SUCCESS;
}
