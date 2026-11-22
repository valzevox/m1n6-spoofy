---
name: pubg-telemetry-cleaner
description: Clean PUBG and Steam trace vectors, reset NTFS/FAT32 volume IDs, and prevent 24-hour investigation ban chaining. Use when researching PUBG anti-cheat, cleaning game traces, or bypassing account-linking flags.
---

# PUBG Telemetry Cleaner & Anti-Cheat Trace Removal

## Overview
PUBG employs a 3-layer anti-cheat architecture:
1. **BattlEye** (`BEDrive.sys`, `BEService.exe`): Kernel memory scanner.
2. **Wellbia XIGNCODE3** (`xhunter1.sys`, `x3.xem`): Syscall integrity and driver whitelist.
3. **Zakynthos / Uncheater** (`TslGame_ZK.exe`, `ucldr_battlegrounds_gl.exe`): Proprietary telemetry collector and hardware fingerprint engine.

Alt accounts receive 24-hour temporary bans ("investigation bans") when Zakynthos links them to previously flagged or banned accounts via traces left in Steam configuration files, registry keys, volume serials, or ARP cache.

## Process

### 1. Process Termination
Terminate all related processes before touching files:
- `TslGame.exe`, `TslGame_ZK.exe`, `TslGame_BE.exe`, `TslGame_UC.exe`
- `ExecPubg.exe`, `BEService.exe`, `xhunter1.sys` / `x3.xem`
- `steam.exe`, `steamwebhelper.exe`

### 2. Disk & Cache Trace Purge
Purge all telemetry, crash dumps, and local caches:
- `%LOCALAPPDATA%\TslGame\Saved\Demos`
- `%LOCALAPPDATA%\TslGame\Saved\Logs`
- `%LOCALAPPDATA%\TslGame\Saved\Crashes`
- `%COMMONPROGRAMFILES(x86)%\BattlEye`
- `C:\ProgramData\Wellbia.com`

### 3. Steam Account Unlinking
Remove Steam persistent tokens that link banned accounts to new accounts:
- Delete `C:\Program Files (x86)\Steam\config\loginusers.vdf`
- Delete `C:\Program Files (x86)\Steam\config\config.vdf`
- Purge `C:\Program Files (x86)\Steam\userdata\*`
- Delete Registry keys:
  - `HKCU\Software\Valve\Steam\Users`
  - `HKCU\Software\Valve\Steam\ActiveProcess` (User values)

### 4. Volume Serial Number (NTFS/FAT32) Modification
Zakynthos records the raw Volume Serial Number of game drives:
- Open volume handle via Win32 `CreateFile` with `GENERIC_READ | GENERIC_WRITE`.
- Read Sector 0 (Boot Sector).
- Modify the 4-byte Volume ID offset (NTFS: offset `0x48`, FAT32: offset `0x43`).
- Write sector back and flush file buffers.

### 5. Network Stack Refresh
- Flush DNS resolver cache: `ipconfig /flushdns`
- Flush ARP table: `netsh interface ip delete arpcache`
