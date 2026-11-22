/*
 * m1n6-EFI / disk.h
 * Language: C (UEFI EDK2 Standard)
 * Description: Disk and BlockIO interception definitions
 */

#ifndef M1N6_DISK_H
#define M1N6_DISK_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskInfo.h>

EFI_STATUS
M1n6HookDiskProtocols(VOID);

#endif // M1N6_DISK_H
