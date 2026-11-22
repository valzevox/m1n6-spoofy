# ⚙️ Spoofing Module Logic & Algorithms

Parent: [[docs/00_Index_MOC]]  
Related: [[docs/architecture/overview]] | [[docs/registry/hardware_keys]]

---

## 1. Random Generators

Dự án sử dụng các hàm helper tự tạo để sinh identifier:

### a. `RandomId(int length)`
- Ký tự sử dụng: `ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`
- Ứng dụng: Tạo Serial ổ đĩa (14 ký tự), SMBIOS Serial (10 ký tự), PC Name suffix (7-8 ký tự).

### b. `RandomMac()`
- Ký tự hợp lệ cho byte đầu tiên: byte thứ hai bắt buộc phải thuộc tập hợp `2, 6, A, E` để đảm bảo cờ bit **Locally Administered Address (LAA)** của chuẩn IEEE 802 MAC address:
  ```csharp
  string chars = "ABCDEF0123456789";
  string windows = "26AE";
  result += chars[random.Next(chars.Length)];
  result += windows[random.Next(windows.Length)]; // Bit locally administered
  ```
- Định dạng xuất: `XX-XX-XX-XX-XX-XX`.

### c. `RandomIdprid(int length)` & `RandomIdprid2(int length)`
- Tạo định dạng chuẩn Microsoft Product Key / Custom UUID: Chèn dấu gạch ngang `-` tại các vị trí index 8, 13, 18, 23 hoặc index 5, 11, 17, 23.

---

## 2. Quy trình áp dụng Network Interface

Sau khi ghi `NetworkAddress` vào Registry của Adapter, thay đổi sẽ không có hiệu lực ngay nếu không khởi động lại adapter:

```csharp
// Ghi registry
NetworkAdapter.SetValue("NetworkAddress", macAfter);

// Áp dụng qua netsh CLI
LocalAreaConection(adapterId, false); // netsh interface set interface "<Name>" disable
LocalAreaConection(adapterId, true);  // netsh interface set interface "<Name>" enable
```

---

## 3. Danh sách giả lập mẫu Disk Hardware (`diskNames`)

Trong `Spoofy.cs`, hệ thống giả lập tên mẫu thiết bị từ các thương hiệu phổ biến:
- `Samsung SSD 870 QVO 1TB`
- `NVMe KINGSTON SA2000M2105`
- `Crucial MX500 1TB`
- `WD Blue 2TB`
- `Seagate Barracuda 4TB`
- `Samsung 970 EVO Plus 1TB`
- ...
Ghi thẳng vào value `Identifier` của nhánh `Scsi` và `IDE`.
