/*
 * m1n6-EFI / smbios.h
 * Language: C (UEFI EDK2 Standard)
 * Description: SMBIOS Structure definitions & spoofing prototypes
 */

#ifndef M1N6_SMBIOS_H
#define M1N6_SMBIOS_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/SmBios.h>

#pragma pack(1)

typedef struct {
    UINT8   Type;
    UINT8   Length;
    UINT16  Handle;
} SMBIOS_STRUCTURE_HEADER;

typedef struct {
    SMBIOS_STRUCTURE_HEADER Header;
    SMBIOS_TABLE_STRING     Manufacturer;
    SMBIOS_TABLE_STRING     ProductName;
    SMBIOS_TABLE_STRING     Version;
    SMBIOS_TABLE_STRING     SerialNumber;
    EFI_GUID                Uuid;
    UINT8                   WakeUpType;
    SMBIOS_TABLE_STRING     SKUNumber;
    SMBIOS_TABLE_STRING     Family;
} SMBIOS_TYPE1_SYSTEM_INFO;

typedef struct {
    SMBIOS_STRUCTURE_HEADER Header;
    SMBIOS_TABLE_STRING     Manufacturer;
    SMBIOS_TABLE_STRING     ProductName;
    SMBIOS_TABLE_STRING     Version;
    SMBIOS_TABLE_STRING     SerialNumber;
    SMBIOS_TABLE_STRING     AssetTag;
    UINT8                   FeatureFlag;
    SMBIOS_TABLE_STRING     LocationInChassis;
    UINT16                  ChassisHandle;
    UINT8                   BoardType;
} SMBIOS_TYPE2_BASEBOARD_INFO;

typedef struct {
    SMBIOS_STRUCTURE_HEADER Header;
    SMBIOS_TABLE_STRING     Manufacturer;
    UINT8                   Type;
    SMBIOS_TABLE_STRING     Version;
    SMBIOS_TABLE_STRING     SerialNumber;
    SMBIOS_TABLE_STRING     AssetTag;
} SMBIOS_TYPE3_CHASSIS_INFO;

#pragma pack()

EFI_STATUS
M1n6SpoofSmbiosTables(
    IN EFI_SYSTEM_TABLE *SystemTable
);

CHAR8*
M1n6GetSmbiosString(
    IN SMBIOS_STRUCTURE_HEADER *Header,
    IN SMBIOS_TABLE_STRING StringNumber
);

VOID
M1n6RandomizeBuffer(
    IN OUT UINT8 *Buffer,
    IN UINTN Length
);

#endif // M1N6_SMBIOS_H
