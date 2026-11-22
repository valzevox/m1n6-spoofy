# 🛡️ Phân Tích Chuyên Sâu: Kernel Anti-Cheat Ban Telemetry Vectors

Parent: [[docs/00_Index_MOC]]  
Related: [[docs/architecture/overview]] | [[docs/architecture/uefi_bootloader]] | [[docs/registry/hardware_keys]]

---

## 1. Tổng Quan Cơ Chế Thu Thập HWID Của Kernel Anti-Cheat (Ring 0)

Các hệ thống Anti-Cheat hàng đầu hiện nay (**Riot Vanguard, Tencent ACE, Epic Easy Anti-Cheat, BattlEye, Call of Duty Ricochet**) đều sở hữu driver kernel chạy ở cấp độ Ring 0 ngay từ khi boot Windows. 

Chúng **hoàn toàn bỏ qua các giá trị Registry ở User-Mode** và trực tiếp truy vấn phần cứng:

```mermaid
graph TD
    AC[Kernel Anti-Cheat Driver (Ring 0)]
    AC -->|Physical Memory / EFI Table| SMBIOS[SMBIOS Table (UUID, Serial, Board)]
    AC -->|IOCTL / IRP Direct Pass-through| DISK[NVMe / SATA Physical Serials]
    AC -->|NDIS OID Query| NIC[Permanent MAC from EEPROM]
    AC -->|I2C / DDC Bus Query| MON[Monitor EDID Serial]
    AC -->|Tbs.sys / TPM API| TPM[TPM 2.0 EK Certificate & AIK]
    AC -->|CPUID / MSRs| CPU[Processor Serial / Features]
```

---

## 2. Chi Tiết Các Vector Telemetry Bị Ban

### 2.1. Ổ đĩa vật lý (Storage Hardware ID)
- **Truy vấn**: Gửi trực tiếp các lệnh IOCTL:
  - `IOCTL_STORAGE_QUERY_PROPERTY` với `PropertyId = StorageDeviceProperty`.
  - `SMART_RCV_DRIVE_DATA` (ATA Identify Command).
  - SCSI Miniport `IOCTL_SCSI_MINIPORT`.
- **Dữ liệu thu thập**:
  - Raw Serial Number trong firmware controller ổ cứng (không phải Volume Serial Number do format đĩa tạo ra).
  - NVMe Controller Model / Serial từ `NVME_IDENTIFY_CONTROLLER_DATA`.
- **Hạn chế User-Mode**: Việc sửa registry `HARDWARE\DEVICEMAP\Scsi` chỉ đánh lừa được hàm Win32 API thông thường, không thể đánh lừa kernel driver của AC.

### 2.2. Bo Mạch Chủ & SMBIOS (DMI / Firmware Table)
- **Truy vấn**:
  - Đọc bảng cấu trúc SMBIOS từ vùng nhớ vật lý `0x000F0000 - 0x000FFFFF` hoặc qua con trỏ UEFI Configuration Table.
  - Gọi `NtQuerySystemInformation(SystemFirmwareTableInformation)`.
- **Dữ liệu thu thập**:
  - **Type 1 (System Information)**: `UUID` (128-bit định danh máy), `Serial Number`, `Wake-up Type`.
  - **Type 2 (Baseboard Information)**: Serial của Motherboard, Board Manufacturer.
  - **Type 3 (Chassis Information)**: Chassis Serial, Enclosure Type.
  - **Type 0 (BIOS Information)**: BIOS Vendor, Version, Release Date.

### 2.3. Card Mạng (Network Interface Controller)
- **Truy vấn**:
  - Gửi truy vấn NDIS OID: `OID_802_3_PERMANENT_ADDRESS` (địa chỉ MAC cứng ghi trong ROM/EEPROM card mạng).
- **Dữ liệu thu thập**:
  - Permanent MAC (địa chỉ MAC gốc của phần cứng).
  - MAC của Default Gateway (Router) thông qua bảng ARP: `GetAdaptersInfo` / kernel NDIS miniport.
  - BSSID của các mạng Wi-Fi lân cận.

### 2.4. TPM 2.0 (Trusted Platform Module) & Secure Boot
- **Đặc trưng trên Windows 11**:
  - Vanguard và Tencent ACE trên Windows 11 (23H2 / 24H2) bắt buộc kích hoạt TPM 2.0 và Secure Boot.
  - Thu thập **EK (Endorsement Key) Certificate**: Khóa mã hóa bất biến được nhúng vào chip TPM từ khi xuất xưởng bởi nhà sản xuất (Intel fTPM, AMD fTPM, Nuvoton, Infineon).
  - Thu thập PCR (Platform Configuration Registers) hash trạng thái khởi động.

### 2.5. Màn hình (Display EDID)
- **Truy vấn**: Đọc trực tiếp cấu trúc EDID (Extended Display Identification Data) qua bus I2C/DDC của card đồ họa.
- **Dữ liệu thu thập**: Monitor Serial Number, Manufacture Week/Year, Panel Model ID.

---

## 3. Vì Sao Giải Pháp UEFI Bootloader (m1n6-EFI) Là Tối Ưu Nhất Cho Win 11?

1. **Thực thi trước Windows Kernel**:
   - EFI driver tải và chạy từ giai đoạn DXE (Driver Execution Environment) của UEFI firmware.
   - Lúc này Windows Kernel (`ntoskrnl.exe`), Hypervisor, và driver Anti-Cheat (`vgk.sys`, `ace-base.sys`) **chưa tồn tại trong bộ nhớ**.

2. **Can thiệp gốc rễ**:
   - Khi EFI driver hook bảng SMBIOS trong EFI Configuration Table hoặc chặn giao thức `EFI_BLOCK_IO_PROTOCOL` / `EFI_STORAGE_SECURITY_COMMAND_PROTOCOL`, dữ liệu trả về cho Windows kernel đã bị làm giả hoàn toàn ngay từ tầng vật lý.
   - Không cần dùng bypass KDU / Vulnerable Driver (vốn bị Windows 11 23H2/24H2 chặn thẳng tay bởi HVCI / VBS và Microsoft Blocklist).
