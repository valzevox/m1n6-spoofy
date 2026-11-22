# 📜 Coding Standards & C# Guidelines

Parent: [[docs/00_Index_MOC]]  
Related: [[docs/rules/project_rules]]

---

## 1. Resource Management & Disposables

- **Sử dụng C# `using` declaration**:
  Mọi instance của `RegistryKey`, `Process`, `StreamReader`, `StreamWriter`, `ServiceController` phải được bọc trong `using` hoặc khối `using var ...;` để giải phóng handle ngay lập tức:
  ```csharp
  using RegistryKey key = Registry.LocalMachine.OpenSubKey(path, writable: true);
  if (key != null)
  {
      key.SetValue(name, value);
  }
  ```

---

## 2. Process Execution Standards

- Khi gọi CLI bên ngoài (`netsh`, `ipconfig`, `wevtutil`, `schtasks`):
  - Luôn cấu hình `CreateNoWindow = true`, `UseShellExecute = false`.
  - Luôn gọi `WaitForExit()` (hoặc `WaitForExitAsync()` khi làm việc với UI async) để tránh tình trạng zombie process hoặc race condition làm treo network.
  - Xử lý output hoặc error stream đầy đủ.

---

## 3. UI Threading & Responsiveness

- Không block UI thread trong Windows Forms:
  - Các thao tác I/O nặng (duyệt cây thư mục temp, ping network, gọi shell) nên được chạy bằng `Task.Run()` hoặc `async/await`.
  - Timer hoạt động ở UI phải được kiểm soát cờ `isDisposed` hoặc huỷ đăng ký event tick khi form đóng.
