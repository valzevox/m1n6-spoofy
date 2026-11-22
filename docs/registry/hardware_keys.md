# 🗄️ Hardware Registry Mapping & Vectors

Parent: [[docs/00_Index_MOC]]  
Related: [[docs/architecture/overview]] | [[docs/modules/spoofing]]

Tất cả các định danh phần cứng mục tiêu được chỉnh sửa tại User-Mode thông qua các nhánh Registry Windows:

---

## 1. Disk & Controller Identifiers

| Thành phần | Đường dẫn Registry (HKLM) | Tên Value | Kiểu dữ liệu | Mô tả |
| :--- | :--- | :--- | :--- | :--- |
| **SCSI Port / Bus** | `HARDWARE\DEVICEMAP\Scsi\{port}\{bus}\Target Id 0\Logical Unit Id 0` | `Identifier`, `SerialNumber` | `REG_SZ` | Chuỗi nhận dạng ổ đĩa SCSI |
| **SCSI Binary Pages** | `HARDWARE\DEVICEMAP\Scsi\{port}\{bus}\Target Id 0\Logical Unit Id 0` | `DeviceIdentifierPage`, `InquiryData` | `REG_BINARY` | Mã hóa byte UTF8 của Serial & Model |
| **Multifunction Disk** | `HARDWARE\DESCRIPTION\System\MultifunctionAdapter\0\DiskController\0\DiskPeripheral\{disk}` | `Identifier` | `REG_SZ` | Format: `{Random8}-{Random8}-A` |
| **IDE Controller** | `SYSTEM\CurrentControlSet\Enum\IDE\{controller}\{disk}` | `SerialNumber` | `REG_SZ` | Serial ổ cứng chuẩn IDE |

---

## 2. Network & MAC Address

| Mục tiêu | Đường dẫn Registry (HKLM) | Tên Value | Cơ chế cập nhật |
| :--- | :--- | :--- | :--- |
| **Network Adapters** | `SYSTEM\CurrentControlSet\Control\Class\{4d36e972-e325-11ce-bfc1-08002be10318}\{adapter}` | `NetworkAddress` | Ghi địa chỉ MAC ngẫu nhiên (Format `X[2|6|A|E]...`). Sau đó gọi `netsh interface set interface "<name>" disable/enable` |

---

## 3. System GUIDs & Identifiers

| Định danh | Đường dẫn Registry (HKLM) | Tên Value | Định dạng giá trị |
| :--- | :--- | :--- | :--- |
| **Hardware Profile** | `SYSTEM\CurrentControlSet\Control\IDConfigDB\Hardware Profiles\0001` | `HwProfileGuid` | `{UUID}` |
| **Cryptography MachineGuid** | `SOFTWARE\Microsoft\Cryptography` | `MachineGuid` | `UUID` (hoặc chuỗi alphanumeric 32 ký tự ngẫu nhiên) |
| **SQM Client** | `SOFTWARE\Microsoft\SQMClient` | `MachineId` | `{UUID}` |
| **Installation ID** | `SOFTWARE\Microsoft\Windows NT\CurrentVersion` | `InstallationID` | `UUID` |
| **Product ID** | `SOFTWARE\Microsoft\Windows NT\CurrentVersion` | `ProductId` | Format: `XXXXX-XXXXX-XXXXX-XXXXX` |
| **Windows Update Client** | `SOFTWARE\Microsoft\Windows\CurrentVersion\WindowsUpdate` | `SusClientId`, `SusClientIdValidation` | GUID & Binary string |

---

## 4. BIOS & SMBIOS

| Mục tiêu | Đường dẫn Registry (HKLM) | Tên Value |
| :--- | :--- | :--- |
| **SMBIOS Serial Number** | `HARDWARE\DESCRIPTION\System\BIOS` | `SystemSerialNumber` |
| **System Info** | `SYSTEM\CurrentControlSet\Control\SystemInformation` | `BIOSReleaseDate`, `BIOSVersion`, `ComputerHardwareId`, `SystemManufacturer`, `SystemProductName` |
| **EFI Variables** | `SYSTEM\CurrentControlSet\Control\Nsi\{eb004a03-9b1a-11d4-9123-0050047759bc}\26` | `VariableId` |

---

## 5. Computer Name & Hostname

| Mục tiêu | Đường dẫn Registry (HKLM) | Tên Value |
| :--- | :--- | :--- |
| **ComputerName** | `SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName` | `ComputerName`, `ActiveComputerName` |
| **ActiveComputerName** | `SYSTEM\CurrentControlSet\Control\ComputerName\ActiveComputerName` | `ComputerName`, `ActiveComputerName` |
| **TCP/IP Parameters** | `SYSTEM\CurrentControlSet\Services\Tcpip\Parameters` | `Hostname`, `NV Hostname` |
| **TCP/IP Interfaces** | `SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\Interfaces\{adapter}` | `Hostname`, `NV Hostname` |
