# 🗺️ m1n6-spoofy Knowledge Base & Architecture MOC

> **Project**: m1n6-spoofy (Next-Gen HWID Changer & Cleaner)  
> **Platform**: Windows 10/11 x64 (.NET 6.0 C# & UEFI EDK2 C)  
> **Vault Root**: `[[docs/00_Index_MOC]]`

---

## 📌 Quick Links (Obsidian Wiki-Links)

- **Architecture Overview**: [[docs/architecture/overview]]
- **Kernel Anti-Cheat Analysis (Vanguard, ACE, EAC)**: [[docs/architecture/kernel_anticheat_analysis]]
- **UEFI Bootloader Engine (m1n6-EFI)**: [[docs/architecture/uefi_bootloader]]
- **Component & Execution Flow**: [[docs/architecture/components]]
- **Registry & Hardware Vectors**: [[docs/registry/hardware_keys]]
- **Spoofing Logic**: [[docs/modules/spoofing]]
- **Cleaner & Trace Removal**: [[docs/modules/cleaner]]
- **Development & AI Rules**: [[docs/rules/project_rules]]
- **Code Standards & Constraints**: [[docs/rules/coding_standards]]

---

## 🗂️ Structural Map

```mermaid
graph TD
    MOC[00_Index_MOC] --> ARCH[Architecture Overview]
    MOC --> KERNEL[Kernel Anti-Cheat Analysis]
    MOC --> EFI[m1n6-EFI Bootloader]
    MOC --> REG[Registry & Vectors]
    MOC --> MOD[Core Modules]
    MOC --> RULES[Project Rules]

    KERNEL --> AC[Vanguard / ACE / EAC Telemetry]
    EFI --> BOOT[DXE Pre-OS SMBIOS / Disk Hooks]
    ARCH --> COMP[Components & Data Flow]
    MOD --> SPOOF[Spoofing Engine]
    MOD --> CLEAN[Cleaner & Process Terminator]
    REG --> KEYS[Hardware Registry Keys]
    RULES --> CODE[Coding Standards]
```

---

## 📂 Vault Structure

- `docs/architecture/`
  - [[docs/architecture/overview]]: Tổng quan kiến trúc hệ thống, phân tầng CLI vs GUI, ranh giới UserMode vs Ring 0.
  - [[docs/architecture/kernel_anticheat_analysis]]: Phân tích chuyên sâu các vector ban phần cứng của Vanguard, Tencent ACE, EAC, BattlEye trên Windows 11 (23H2 / 24H2).
  - [[docs/architecture/uefi_bootloader]]: Thiết kế kỹ thuật module EFI Bootloader can thiệp SMBIOS và Disk Table trước khi Windows boot.
  - [[docs/architecture/components]]: Luồng dữ liệu, tiến trình thực thi, lifecycle và dependency.
- `docs/registry/`
  - [[docs/registry/hardware_keys]]: Toàn bộ sơ đồ các nhánh Registry Windows bị tác động.
- `docs/modules/`
  - [[docs/modules/spoofing]]: Phân tích chi tiết thuật toán sinh ID giả mạo, Network restart, Disk spoofing.
  - [[docs/modules/cleaner]]: Cơ chế xóa logs, cache game launcher, terminate anti-cheat.
- `docs/rules/`
  - [[docs/rules/project_rules]]: Bộ nguyên tắc dự án, bảo mật, xử lý UAC / Admin permissions.
  - [[docs/rules/coding_standards]]: Quy chuẩn code C# .NET 6 và C UEFI.
