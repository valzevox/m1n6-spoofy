# ROB RUN — Easy Anti-Cheat (EAC) Architecture & Bypass Analysis

> [!NOTE]
> Game: **ROB RUN** (Steam Release, Tháng 11/2026)  
> Anti-Cheat: **Easy Anti-Cheat (EAC)** cấp Kernel (`EasyAntiCheat_EOS.sys`)  
> Mục tiêu: Phân tích cơ chế cấm HWID, vector theo dõi của EAC trên ROB RUN và phương thức xóa sạch trace.

---

## 1. Tổng quan Anti-Cheat của ROB RUN

ROB RUN là tựa game nhiều người chơi mới ra mắt trên Steam vào tháng 11/2026. Do tình trạng gian lận bùng nổ ngay sau khi ra mắt, nhà phát triển đã cập nhật phiên bản EAC (Epic Online Services Tier) với các cơ chế kiểm tra nâng cao:

1. **Kernel Driver**: `EasyAntiCheat_EOS.sys` / `EasyAntiCheat.sys`.
2. **Server-side Heartbeat**: Kiểm tra tính toàn vẹn phiên chơi định kỳ. Nếu giả lập EAC (EAC emulator) thiếu token hợp lệ từ server Epic Games, game sẽ ngắt kết nối với thông báo: *"Connection terminated by anti-cheat"* sau 2 - 3 phút.
3. **HWID Fingerprinting**:
   - Truy vấn Serial ổ đĩa (NVMe / SATA Identify qua SCSI miniport).
   - Truy vấn SMBIOS UUID của bo mạch chủ.
   - Truy vấn MAC Address card mạng và ARP Cache router.
   - Volume Serial Number (NTFS Volume ID).

---

## 2. Các vector liên kết tài khoản (Account Chaining Vectors)

Khi bị cấm trên ROB RUN, EAC lưu trữ cờ nhận diện ở các vị trí:

| Vector | Vị trí lưu trữ | Tác động |
| :--- | :--- | :--- |
| **Steam Account Token** | `Steam\config\loginusers.vdf` | Liên kết Steam ID mới với Steam ID cũ đã bị cấm |
| **EAC Client Cache** | `%APPDATA%\EasyAntiCheat\` | Dữ liệu chứng chỉ và telemetry phiên chơi cũ |
| **EAC System Data** | `C:\ProgramData\EasyAntiCheat\` | Bản ghi xác thực phần cứng cấp máy |
| **Game Logs & Dumps** | `%LOCALAPPDATA%\RobRun\` | Crash dumps, thông số cấu hình và hardware snapshot |
| **Network Identity** | Windows ARP Cache | MAC Address của Default Gateway / Router |

---

## 3. Quy trình xử lý sạch sẽ (Clean & Spoof Procedure)

1. **Dừng tiến trình**: Kết thúc `RobRun.exe`, `RobRun-Win64-Shipping.exe`, `EasyAntiCheat.exe`, `EasyAntiCheat_EOS.exe`, `steam.exe`.
2. **Xóa toàn bộ thư mục Cache**:
   - `%LOCALAPPDATA%\RobRun`
   - `%APPDATA%\EasyAntiCheat`
   - `C:\ProgramData\EasyAntiCheat`
   - `%PROGRAMFILES(x86)%\EasyAntiCheat_EOS`
3. **Hủy liên kết Steam**: Xóa `loginusers.vdf`, `config.vdf`, thư mục `userdata` và Registry `HKCU\Software\Valve\Steam\Users`.
4. **Đổi Volume ID**: Chạy `VolumeIdChanger` đổi Serial phân vùng C: và D:.
5. **Đổi MAC & Flush Network**: Xóa ARP table (`arp -d *`), làm mới DNS (`ipconfig /flushdns`).
6. **Khởi động lại máy & đổi IP mạng**: Tắt bật lại Router mạng để nhận Public IP mới trước khi đăng nhập Steam tài khoản mới.
