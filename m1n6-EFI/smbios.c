/*
 * m1n6-EFI / smbios.c
 * Language: C (UEFI EDK2 Standard)
 * Description: Scanning and modifying SMBIOS Type 1, 2, 3 in UEFI memory
 */

#include "smbios.h"

STATIC UINT64 gSeed = 0x8A5B3C4D5E6F7081ULL;

STATIC
UINT32
M1n6Random32(VOID)
{
    gSeed = (gSeed * 6364136223846793005ULL) + 1442695040888963407ULL;
    return (UINT32)(gSeed >> 32);
}

VOID
M1n6RandomizeBuffer(
    IN OUT UINT8 *Buffer,
    IN UINTN Length
)
{
    CONST CHAR8 CharSet[] = "0123456789ABCDEF";
    UINTN i;

    if (Buffer == NULL || Length == 0) {
        return;
    }

    for (i = 0; i < Length; i++) {
        Buffer[i] = CharSet[M1n6Random32() % (sizeof(CharSet) - 1)];
    }
}

CHAR8*
M1n6GetSmbiosString(
    IN SMBIOS_STRUCTURE_HEADER *Header,
    IN SMBIOS_TABLE_STRING StringNumber
)
{
    CHAR8 *Str;
    UINT8 Idx;

    if (StringNumber == 0 || Header == NULL) {
        return NULL;
    }

    Str = (CHAR8*)((UINT8*)Header + Header->Length);
    Idx = 1;

    while (Idx < StringNumber && *Str != '\0') {
        while (*Str != '\0') {
            Str++;
        }
        Str++;
        Idx++;
    }

    return (*Str == '\0') ? NULL : Str;
}

EFI_STATUS
M1n6SpoofSmbiosTables(
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINTN i;
    SMBIOS_TABLE_ENTRY_POINT *SmbiosEntry = NULL;
    UINT8 *Record;
    SMBIOS_STRUCTURE_HEADER *Header;

    if (SystemTable == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    /* Locate SMBIOS Table Pointer in Configuration Table */
    for (i = 0; i < SystemTable->NumberOfTableEntries; i++) {
        if (CompareGuid(&SystemTable->ConfigurationTable[i].VendorGuid, &gEfiSmbiosTableGuid)) {
            SmbiosEntry = (SMBIOS_TABLE_ENTRY_POINT*)SystemTable->ConfigurationTable[i].VendorTable;
            break;
        }
    }

    if (SmbiosEntry == NULL || SmbiosEntry->TableAddress == 0) {
        Print(L"[m1n6-EFI] SMBIOS Table Entry Point not found\n");
        return EFI_NOT_FOUND;
    }

    Record = (UINT8*)(UINTN)SmbiosEntry->TableAddress;

    while ((UINTN)Record < (SmbiosEntry->TableAddress + SmbiosEntry->TableLength)) {
        Header = (SMBIOS_STRUCTURE_HEADER*)Record;

        /* Type 127 indicates end of SMBIOS table */
        if (Header->Type == 127) {
            break;
        }

        /* Type 1: System Information (UUID & System Serial Number) */
        if (Header->Type == 1 && Header->Length >= sizeof(SMBIOS_TYPE1_SYSTEM_INFO)) {
            SMBIOS_TYPE1_SYSTEM_INFO *Type1 = (SMBIOS_TYPE1_SYSTEM_INFO*)Header;
            CHAR8 *Serial = M1n6GetSmbiosString(Header, Type1->SerialNumber);

            if (Serial != NULL) {
                M1n6RandomizeBuffer((UINT8*)Serial, AsciiStrLen(Serial));
            }

            /* Randomize System UUID (16 bytes) */
            for (i = 0; i < sizeof(EFI_GUID); i++) {
                ((UINT8*)&Type1->Uuid)[i] = (UINT8)(M1n6Random32() & 0xFF);
            }
        }

        /* Type 2: Baseboard / Motherboard Information */
        if (Header->Type == 2 && Header->Length >= sizeof(SMBIOS_TYPE2_BASEBOARD_INFO)) {
            SMBIOS_TYPE2_BASEBOARD_INFO *Type2 = (SMBIOS_TYPE2_BASEBOARD_INFO*)Header;
            CHAR8 *Serial = M1n6GetSmbiosString(Header, Type2->SerialNumber);

            if (Serial != NULL) {
                M1n6RandomizeBuffer((UINT8*)Serial, AsciiStrLen(Serial));
            }
        }

        /* Type 3: Chassis Information */
        if (Header->Type == 3 && Header->Length >= sizeof(SMBIOS_TYPE3_CHASSIS_INFO)) {
            SMBIOS_TYPE3_CHASSIS_INFO *Type3 = (SMBIOS_TYPE3_CHASSIS_INFO*)Header;
            CHAR8 *Serial = M1n6GetSmbiosString(Header, Type3->SerialNumber);

            if (Serial != NULL) {
                M1n6RandomizeBuffer((UINT8*)Serial, AsciiStrLen(Serial));
            }
        }

        /* Advance to next SMBIOS record (skipping null-terminated strings) */
        Record += Header->Length;
        while (*(UINT16*)Record != 0) {
            Record++;
        }
        Record += 2;
    }

    Print(L"[m1n6-EFI] SMBIOS Tables successfully spoofed\n");
    return EFI_SUCCESS;
}
