# 🧹 Cleaner & Trace Removal Engine

Parent: [[docs/00_Index_MOC]]  
Related: [[docs/architecture/overview]] | [[docs/architecture/components]]

Module `cleaner.cs` (South Africa window) chịu trách nhiệm vô hiệu hóa telemetry, xóa dấu vết định danh và diệt các process Anti-Cheat đang chạy.

---

## 1. Dọn dẹp Network & DNS

- **DNS Flush**: Thực thi CLI `ipconfig /flushdns`.
- **TCP/IP Reset**: Thực thi CLI `netsh int ip reset`.
- **Full Network Reset (`rstreset()`)**:
  - Cấu hình lại các Windows Services mạng (`Dhcp`, `DPS`, `NlaSvc`, `nsi`, `RmSvc`, `Wcmsvc`, `Winmgmt`...).
  - Vô hiệu hoá và kích hoạt lại các network adapter bằng `wmic path win32_networkadapter where index={0..5} call disable/enable`.
  - Xóa bảng ARP: `arp -d *`.
  - Xóa route: `route -f`.
  - Reset NetBIOS: `nbtstat -R`, `nbtstat -RR`.
  - Reset Winsock & Windows Firewall: `netsh advfirewall reset`, `netsh winsock reset`.

---

## 2. Dọn dẹp Filesystem & Logs

- **Windows Event Logs**: Chạy `wevtutil el` xóa toàn bộ event logs.
- **Temp Cache**: Xóa toàn bộ file và folder con trong `%TEMP%` (`Path.GetTempPath()`).
- **Browser Cookies**:
  - Chrome: Xoá `%LocalAppData%\Google\Chrome\User Data\Default\Cookies`.
  - Firefox: Xoá `cookies.sqlite` trong `%AppData%\Mozilla\Firefox\Profiles\{profile}`.
- **Game Launcher Traces**:
  - Blizzard / Battle.net: Xóa thư mục cache, WidevineCdm, GPUCache, Logs trong `%LocalAppData%`, `%AppData%`, `%ProgramData%`.
  - Call of Duty: Xóa file profile trong `Documents\Call of Duty Modern Warfare` và `Documents\Call of Duty Black Ops Cold War`.
  - Ubisoft: Xoá cache, logs, savegames, spool.
  - Valorant: Xoá `%LocalAppData%\VALORANT\saved`.

---

## 3. Anti-Cheat Terminator & Unlinkers

- **Kill Processes**: Dùng `Process.GetProcessesByName(name).Kill()` đối với:
  - `BEService.exe`, `EasyAntiCheat.exe`, `FortniteClient-Win64-Shipping.exe`...
  - `FiveM.exe`, `FiveM_b2802_GTAProcess.exe`, `Steam.exe`, `EpicGamesLauncher.exe`...
  - `Vanguard`, `PunkBuster`, `ricocheat`.
- **Stop Services**: Sử dụng `System.ServiceProcess.ServiceController` để stop `BEService`, `EasyAntiCheat`, `Vanguard`, `ricocheat`.
- **Unlink Xbox**:
  - Gỡ Appx: `Get-AppxPackage -AllUsers xbox | Remove-AppxPackage`.
  - Xoá dịch vụ `XblAuthManager`, `XblGameSave`, `XboxNetApiSvc`, `XboxGipSvc`.
  - Chặn DNS telemetry trong `C:\Windows\System32\drivers\etc\hosts`:
    - `127.0.0.1 xboxlive.com`
    - `127.0.0.1 user.auth.xboxlive.com`
    - `127.0.0.1 presence-heartbeat.xboxlive.com`
- **Unlink Discord**:
  - Đổi tên module RPC `%LocalAppData%\Discord\app-*\modules\discord_rpc-1` thành `SecHex_Was_Here` để game không thể gửi trạng thái người chơi lên Discord Presence.
