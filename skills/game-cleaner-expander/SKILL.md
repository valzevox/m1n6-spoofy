---
name: game-cleaner-expander
description: Standardized workflow for expanding m1n6-spoofy with new target games (PUBG, Valorant, ROB RUN, Delta Force). Identifies telemetry caches, terminates processes, purges account-linking tokens, and synchronizes GUI and CLI menus.
---

# Game Cleaner & Anti-Cheat Trace Expansion

## Overview
When onboarding a new game target into `m1n6-spoofy`, follow this strict 6-stage lifecycle across `SecHex-GUI` and `SecHex-CMDLINE` to guarantee zero account linkage and full HWID isolation.

## Process

### Stage 1: Process & Kernel Service Termination
- Enumerate both the game executable and all anti-cheat companion watchdogs:
  - Unreal / Unity shipping binaries (e.g. `GameName-Win64-Shipping.exe`).
  - Kernel anti-cheat service processes (e.g. `EasyAntiCheat_EOS.exe`, `SGuard64.exe`, `BEService.exe`).
- Stop matching Windows services before attempting file deletion to prevent file-lock (`EBUSY` / `AccessDenied`) errors.

### Stage 2: Telemetry & Dumps Purge
- Target primary telemetry drop paths:
  - `%LOCALAPPDATA%\<GameName>` (Crash dumps, saved state, hardware hashes)
  - `%APPDATA%\<AntiCheatProvider>` (Certificates, heartbeat tokens)
  - `C:\ProgramData\<AntiCheatProvider>` (System-wide machine registration logs)
  - `%COMMONPROGRAMFILES(x86)%\<AntiCheatProvider>`

### Stage 3: Launcher Account Unlinking
- Anti-cheats chain bans through launcher persistent IDs:
  - Steam: Delete `Steam\config\loginusers.vdf`, `Steam\config\config.vdf`, and `Steam\userdata\*`.
  - Registry: Delete `HKCU\Software\Valve\Steam\Users` and `HKCU\Software\<AntiCheatProvider>`.

### Stage 4: Network Stack Reset
- Flush local resolver and ARP tables to break local router MAC binding:
  - `arp -d *`
  - `ipconfig /flushdns`
  - `ipconfig /release` && `ipconfig /renew`

### Stage 5: Volume Serial Number Randomization
- Always invoke `VolumeIdChanger.SpoofDriveVolumeId('C', ...)` and `('D', ...)` to modify the physical boot sector Volume ID on NTFS/FAT32 partitions.

### Stage 6: Codebase & Menu Synchronization
- Add implementation method in `SecHex-GUI/SecHex-GUI/cleaner.cs`.
- Add CLI method and menu entry in `SecHex-CMDLINE/RPZ-HWID/Program.cs`.
- Include target in `[13] Spoof all` batch execution.
- Build and verify both projects with `dotnet build` (Exit code 0 required).
