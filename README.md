<p align="center">
  <img src="images/m1n6-logo.png" alt="m1n6 logo" width="160" />
</p>

# m1n6-spoofy [1.5.8]

> **Advanced HWID Changer & System Cleaner**  
> Maintained by **@m1n6** ([@valzevox](https://github.com/valzevox))

---

## ⚡ Supported Operating Systems

- **Windows 11** (Version 10.0.22621 Build 22621+) ✅
- **Windows 10** (Version 22H2 Build 19045.2965+) ✅

---

## 🚀 How to Use

1. **Release build**: Run `m1n6-spoofy.exe` with Administrator rights (`Run as administrator`).
2. **From Source**:
   - Open solution in Visual Studio 2022 (`.NET 6.0`).
   - Build in `Release` configuration.
   - Run the executable with Administrator privileges.

---

## 🛡️ Core Features

+ **Disk Spoofing**
  - Scans SCSI ports & buses in Windows Registry.
  - Generates randomized disk identifiers and hardware serials.
  - Updates `HARDWARE\DEVICEMAP\Scsi` & `HARDWARE\DESCRIPTION\System\MultifunctionAdapter`.

+ **GUID & System Identity Spoofing**
  - Generates new UUIDs for `HwProfileGuid`, `MachineGuid`, and `MachineId`.
  - Spoofs BIOS release date, vendor strings, and hardware profile IDs.

+ **PC-Name Spoofing**
  - Generates randomized computer hostname with prefix `m1n6-XXXXXXX`.
  - Synchronizes `ComputerName`, `ActiveComputerName`, `Hostname`, and `NV Hostname`.

+ **MAC Address Spoofing**
  - Identifies active network adapters under NDIS classes.
  - Generates valid IEEE 802 LAA MAC addresses.
  - Automatically resets network adapters via `netsh` to apply new addresses.

+ **m1n6 Cleaner & Trace Removal**
  - **Network**: DNS Flush (`ipconfig /flushdns`), TCP/IP stack reset (`netsh int ip reset`), Winsock reset.
  - **System Logs**: Clears Windows Event Logs (`wevtutil`).
  - **Browser Traces**: Cleans Google Chrome cookies & Firefox SQLite cookie vaults.
  - **Game Launchers**: Removes cache and tracking files for Ubisoft Connect, Riot Games (VALORANT), Blizzard Battle.net, and Call of Duty.
  - **Anti-Cheat Terminator**: Kills background processes and services (EAC, BattlEye, Vanguard, Ricochet).
  - **Telemetry Unlinking**: Unlinks Xbox telemetry services & disables Discord RPC tracking (`m1n6_Was_Here`).

+ **Live Logging & Verification**
  - Real-time before/after audit trail recorded in `Logs/{timestamp}.txt`.
  - Built-in Registry verification tool to confirm target keys before execution.

---

## 📚 Technical Documentation & Vault

The complete technical architecture, registry keys, and internal mechanics are documented in the Obsidian-ready knowledge base:
- **[Documentation MOC](docs/00_Index_MOC.md)**
- **[Architecture Overview](docs/architecture/overview.md)**
- **[Registry Vectors Map](docs/registry/hardware_keys.md)**
- **[Rules & Standards](docs/rules/project_rules.md)**

---

## 👤 Author & Community

- **Author**: m1n6 ([@valzevox](https://github.com/valzevox))
- **Repository**: [https://github.com/valzevox/m1n6-spoofy](https://github.com/valzevox/m1n6-spoofy)
- **Discord**: @m1n6

---

## ⚠️ Disclaimer

This tool is provided for educational, research, and informational purposes only. Modifying hardware identifiers and system registry keys can affect system stability. Use at your own risk. The author assumes no liability for any misuse or damages.
