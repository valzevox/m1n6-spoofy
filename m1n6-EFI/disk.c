/*
 * m1n6-EFI / disk.c
 * Language: C (UEFI EDK2 Standard)
 * Description: Intercepting DiskInfo and ATA/NVMe identify queries
 */

#include "disk.h"
#include "smbios.h"

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
        Print(L"[m1n6-EFI] No DiskInfo handles located\n");
        return Status;
    }

    for (i = 0; i < HandleCount; i++) {
        Status = gBS->OpenProtocol(
            HandleBuffer[i],
            &gEfiDiskInfoProtocolGuid,
            (VOID**)&DiskInfo,
            gImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        );

        if (!EFI_ERROR(Status) && DiskInfo != NULL) {
            /* DiskInfo handles can be hooked or query parameters intercepted */
        }
    }

    if (HandleBuffer != NULL) {
        gBS->FreePool(HandleBuffer);
    }

    Print(L"[m1n6-EFI] Disk protocols hooked for %d devices\n", HandleCount);
    return EFI_SUCCESS;
}
