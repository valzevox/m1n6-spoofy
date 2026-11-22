# 🧩 Component & Execution Flow

Parent: [[docs/00_Index_MOC]]  
Related: [[docs/architecture/overview]] | [[docs/modules/spoofing]] | [[docs/modules/cleaner]]

---

## 1. Vòng đời thực thi (Execution Lifecycle)

```mermaid
sequenceDiagram
    autonumber
    actor User as Operator / Admin
    participant App as SecHex (GUI / CLI)
    participant Reg as Windows Registry
    participant SCM as Service Control Manager
    participant Shell as Windows Shell (netsh / wevtutil)
    participant FS as File System / Logs

    User->>App: Khởi chạy (Require Administrator UAC)
    App->>Reg: Kiểm tra sự tồn tại của các key mục tiêu (Registry Checker)
    alt Thực thi Spoof All
        App->>Reg: Ghi đè Disk Serial, SCSI Bus, IDE Keys
        App->>Reg: Cập nhật MAC Address trên NIC
        App->>Shell: Netsh restart Interface Ethernet
        App->>Reg: Đổi MachineGuid, HwProfileGuid, MachineId, ProductId
        App->>Reg: Đổi SMBIOS SystemSerialNumber, BIOSReleaseDate, PC Name
        App->>FS: Ghi nhật ký Before / After vào Logs/yyyy-MM-dd_HH-mm-ss.txt
    else Thực thi Cleaner & Anti-Cheat Terminator
        App->>SCM: Dừng / Xóa service (EAC, BE, Vanguard)
        App->>Shell: wevtutil xoá Windows Event Logs
        App->>Shell: ipconfig /flushdns, netsh winsock reset
        App->>FS: Xoá game cache, Chrome/Firefox cookies, Discord RPC rename
    end
    App-->>User: Thông báo thành công / Yêu cầu reboot
```

---

## 2. Quản lý Log & Audit (Logging System)

- **Định dạng file**: `Logs/{yyyy-MM-dd_HH-mm-ss}.txt`
- **Cấu trúc bản ghi**:
  ```text
  HH:mm:ss: ID <module_id> - <Value> (Before)
  HH:mm:ss: ID <module_id> - <Value> (After)
  ```
- **Live Stream**: Gửi trực tiếp tới Form `logs.cs` qua hàm `AddLogEntry()`.
