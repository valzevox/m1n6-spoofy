# 🛡️ Project Rules & Operating Guidelines

Parent: [[docs/00_Index_MOC]]  
Related: [[docs/rules/coding_standards]] | [[docs/architecture/overview]]

---

## 1. Security & Privilege Boundary

1. **Mandatory Admin Elevation**:
   - Tất cả các tác vụ thay đổi Registry trong `HKEY_LOCAL_MACHINE` (HKLM), dừng system services và gọi `netsh` / `wevtutil` / `hosts file` đều yêu cầu quyền Administrator cao nhất.
   - Luôn duy trì thẻ `requireAdministrator` trong `app.manifest`:
     ```xml
     <requestedExecutionLevel level="requireAdministrator" uiAccess="false" />
     ```

2. **Registry Safety & Defensive Checks**:
   - Trước khi sửa/xóa bất kỳ Registry Key nào, luôn kiểm tra `null` đối với `RegistryKey` trả về.
   - Bắt buộc bọc thao tác trong khối `try...catch` cụ thể cho `SecurityException` hoặc `UnauthorizedAccessException`.

3. **Backup Before Mutation**:
   - Luôn khuyến nghị người dùng xuất backup file `.reg` trước khi chạy các tính năng "Spoof All".

---

## 2. Platform Constraints

- **Hệ điều hành hỗ trợ**: Windows 10 (Build 19045 trở lên) và Windows 11 (Build 22621 trở lên).
- **Target Runtime**: .NET 6.0 (`net6.0` cho CLI, `net6.0-windows` cho WinForms).
- **Architecture**: `x64` (không nhắm đến x86 do các đường dẫn Registry `WOW6432Node` và cấu trúc driver 64-bit).
- **Không tự ý tích hợp Ring 0 / Kernel driver** nếu chưa có kế hoạch ký số (driver signature) và bypass WHQL/VBS/HVCI.
