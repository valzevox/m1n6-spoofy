# 🗺️ SecHex-Spoofy Knowledge Base & Architecture MOC

> **Project**: SecHex-Spoofy (HWID Changer & Cleaner)  
> **Platform**: Windows 10/11 x64 (.NET 6.0 C#)  
> **Vault Root**: `[[docs/00_Index_MOC]]`

---

## 📌 Quick Links (Obsidian Wiki-Links)

- **Architecture Overview**: [[docs/architecture/overview]]
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
    MOC --> REG[Registry & Vectors]
    MOC --> MOD[Core Modules]
    MOC --> RULES[Project Rules]

    ARCH --> COMP[Components & Data Flow]
    MOD --> SPOOF[Spoofing Engine]
    MOD --> CLEAN[Cleaner & Process Terminator]
    REG --> KEYS[Hardware Registry Keys]
    RULES --> CODE[Coding Standards]
    RULES --> AGENT[AI Agent Guidelines]
```

---

## 📂 Vault Structure

- `docs/architecture/`
  - [[docs/architecture/overview]]: Tổng quan kiến trúc hệ thống, phân tầng CLI vs GUI, ranh giới UserMode.
  - [[docs/architecture/components]]: Luồng dữ liệu, tiến trình thực thi, lifecycle và dependency.
- `docs/registry/`
  - [[docs/registry/hardware_keys]]: Toàn bộ sơ đồ các nhánh Registry Windows bị tác động (SCSI, NIC, SMBIOS, GUIDs, GPU).
- `docs/modules/`
  - [[docs/modules/spoofing]]: Phân tích chi tiết thuật toán sinh ID giả mạo, Network restart, Disk spoofing.
  - [[docs/modules/cleaner]]: Cơ chế xóa logs, cache game launcher (Ubisoft/Valorant/Blizzard), terminate anti-cheat.
- `docs/rules/`
  - [[docs/rules/project_rules]]: Bộ nguyên tắc dự án, bảo mật, xử lý UAC / Admin permissions.
  - [[docs/rules/coding_standards]]: Quy chuẩn code C# .NET 6, naming, exception handling, resource cleanup.
