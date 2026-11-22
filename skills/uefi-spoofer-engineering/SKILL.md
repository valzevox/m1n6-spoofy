---
name: uefi-spoofer-engineering
description: Design, implement, and maintain pre-boot UEFI EDK2 hardware cloaking drivers that intercept disk identify protocols and randomize SMBIOS tables before Windows boot. Use when building kernel anti-cheat bypasses, EFI drivers, or low-level storage hooks.
---

# UEFI Pre-OS Hardware Cloaking & Spoofer Engineering

## Overview
Kernel anti-cheats (Vanguard, BattlEye, EAC) load at early system boot (StartType = 0 or 1) and query storage controllers directly via miniport IRPs, completely bypassing usermode Windows Registry hooks. To counter this, hardware cloaking must execute at the UEFI Boot Services layer before the Windows Boot Manager (`bootmgfw.efi`) loads `ntoskrnl.exe`.

## Key Interception Targets

### 1. Storage Controller Identify Data (NVMe & ATA/SATA)
Anti-cheats send `IOCTL_STORAGE_QUERY_PROPERTY` and `SMART_RCV_DRIVE_DATA` to read hardware controller identity.
- **NVMe Controllers**:
  - Locate handle with `gEfiDiskInfoProtocolGuid` where interface GUID matches `gEfiDiskInfoNvmeInterfaceGuid`.
  - Hook `DiskInfo->Identify` and `DiskInfo->Inquiry`.
  - In the returned NVMe Identify buffer, overwrite the Serial Number at offset `0x04` (length 20 bytes ASCII).
- **ATA/SATA Controllers**:
  - Overwrite Serial Number at offset `0x14` (length 20 bytes ASCII, byte-swapped pairs).

### 2. Physical SMBIOS Table Virtualization
Anti-cheats inspect physical memory (`0xF0000` - `0xFFFFF`) or call `GetSystemFirmwareTable('RSMB')`.
- Scan `SystemTable->ConfigurationTable` for `gEfiSmbiosTableGuid`.
- Traverse SMBIOS records:
  - **Type 1 (System Info)**: Randomize `Uuid` (16 bytes) and replace `SerialNumber` string.
  - **Type 2 (Baseboard Info)**: Replace `SerialNumber` and `AssetTag` strings.
  - **Type 3 (Chassis Info)**: Replace `SerialNumber` string.
- **Mandatory Checksum Recalculation**:
  - Recompute `IntermediateChecksum` over bytes `0x10..0x1E` (length 15).
  - Recompute `EntryPointStructureChecksum` over all bytes `0x00..Length`.

### 3. Chainloader Protocol
- Once hooks and memory patches are in place, chain-load `\EFI\Microsoft\Boot\bootmgfw.efi` via `gBS->LoadImage` and `gBS->StartImage`.
- Keep memory resident or rely on pre-boot table alterations.
