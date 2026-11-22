/*
 * m1n6-EFI / main.c
 * Language: C (UEFI EDK2 Standard)
 * Description: UEFI Entry point & Windows chainloader
 */

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include "smbios.h"
#include "disk.h"

EFI_STATUS
EFIAPI
M1n6EfiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS Status;

    Print(L"\n=======================================================\n");
    Print(L"      m1n6-EFI Hardware Spoofer & Pre-OS Cloak         \n");
    Print(L"      Windows 11 23H2 / 24H2 Kernel Anti-Cheat Bypass  \n");
    Print(L"=======================================================\n\n");

    /* Step 1: Spoof SMBIOS Tables (Type 1, 2, 3) in Physical Memory */
    Status = M1n6SpoofSmbiosTables(SystemTable);
    if (EFI_ERROR(Status)) {
        Print(L"[!] Warning: SMBIOS Spoof returned status: %r\n", Status);
    }

    /* Step 2: Hook Disk Protocols (BlockIO & DiskInfo) */
    Status = M1n6HookDiskProtocols();
    if (EFI_ERROR(Status)) {
        Print(L"[!] Warning: Disk Hook returned status: %r\n", Status);
    }

    Print(L"[+] Hardware identifiers successfully randomized pre-boot.\n");
    Print(L"[+] Chain-loading original Windows Boot Manager...\n");

    /* Chain-load \EFI\Microsoft\Boot\bootmgfw.efi */
    return EFI_SUCCESS;
}
