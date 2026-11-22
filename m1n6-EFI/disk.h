/*
 * m1n6-EFI / disk.h
 * Language: C (UEFI EDK2 Standard)
 * Description: Disk and BlockIO interception definitions for NVMe and SATA
 */

#ifndef M1N6_DISK_H
#define M1N6_DISK_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskInfo.h>

#define NVME_SN_OFFSET       4
#define NVME_SN_LEN          20
#define NVME_MN_OFFSET       24
#define NVME_MN_LEN          40
#define NVME_FR_OFFSET       64
#define NVME_FR_LEN          8

#define ATA_SN_OFFSET        20
#define ATA_SN_LEN           20
#define ATA_MN_OFFSET        54
#define ATA_MN_LEN           40
#define ATA_FR_OFFSET        46
#define ATA_FR_LEN           8

EFI_STATUS
M1n6HookDiskProtocols(VOID);

#endif // M1N6_DISK_H
