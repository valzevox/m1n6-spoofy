# AI Aimbot, Recoil Control & Overlay Bypass Architecture

> [!NOTE]
> Tài liệu kỹ thuật độc lập dành riêng cho nghiên cứu AI Vision Aimbot, Recoil Pattern Control, Mouse Input Injection và Screen Capture / Overlay Hijacking. Tách biệt hoàn toàn khỏi mã nguồn cốt lõi của Spoofer.

---

## 1. Mouse Input Bypass Mechanisms

### 1.1 Hardware-level: KMBox (B+ Pro / Net)
- **KMBox B+ Pro (Serial COM)**:
  - Giao tiếp qua cổng COM (Baudrate 115200).
  - Chuột thật cắm cổng USB IN, dây USB OUT cắm sang máy game.
  - Lệnh gửi: `km.move(x, y)`, `km.left(1)`, `km.right(1)`.
  - Không tồn tại tiến trình hay driver trên máy game can thiệp stack `mouclass.sys` -> An toàn trước Vanguard, EAC, BattlEye.
- **KMBox Net (Ethernet UDP)**:
  - Giao tiếp qua mạng LAN UDP IP/Port.
  - Tốc độ phản hồi cực cao (< 1ms). Phù hợp cho mô hình Dual-PC (PC1 chạy YOLO AI detect, gửi tọa độ qua mạng nội bộ sang KMBox Net cắm ở PC2 chơi game).

### 1.2 Software-level: Logitech G HUB Virtual Driver
- Tận dụng `lgbusenum.sys` và `lgcore.dll`.
- Định danh thiết bị là phần cứng Logitech G chính hãng.
- Cần chạy qua wrapper C# P/Invoke hoặc nhúng script Lua trong profile G HUB để tránh bị quét signature usermode.

---

## 2. Recoil Control Pattern Systems
- **Cơ chế tính toán**:
  - Ghi nhận độ giật của súng theo từng viên đạn (Recoil Table dạng mảng vector offset `(dx, dy, delay_ms)`).
  - Làm mịn chuyển động (Smoothing via Bezier Curve / Humanizer Gaussian noise) để tránh bị hệ thống server-side anti-cheat (như Zakynthos / Ricochet) gắn cờ "Perfect Linear Pattern".

---

## 3. Screen Capture & Overlay Engine

### 3.1 OBS Capture Bypass (`WDA_EXCLUDEFROMCAPTURE`)
- Sử dụng Windows API `SetWindowDisplayAffinity`:
  ```csharp
  [DllImport("user32.dll")]
  public static extern bool SetWindowDisplayAffinity(IntPtr hWnd, uint dwAffinity);
  // dwAffinity = 0x00000011 (WDA_EXCLUDEFROMCAPTURE)
  ```
- DWM loại trừ cửa sổ khỏi mọi luồng chụp màn hình (OBS, Discord, Game Bar).

### 3.2 Overlay Hijacking
- Mượn cửa sổ Direct3D có sẵn của:
  - `DiscordHook64.dll`
  - `GameOverlayRenderer64.dll` (Steam)
  - `nvspcap64.dll` (NVIDIA ShadowPlay)
- Tránh việc tạo một cửa sổ `WS_EX_TOPMOST` lạ vốn là mục tiêu quét số 1 của các phần mềm chống gian lận.
