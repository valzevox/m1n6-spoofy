# 🚀 m1n6-EFI Pre-OS Cloak (UEFI Driver)

## 📌 Tổng Quan
`m1n6-EFI` là module driver UEFI (DXE runtime) thực thi trước khi Windows Kernel (`ntoskrnl.exe`) và các Kernel Anti-Cheat (Riot Vanguard, Tencent ACE, Easy Anti-Cheat, BattlEye) khởi động.

## 🛠️ Hướng Dẫn Biên Dịch (EDK2)

1. **Chuẩn bị môi trường**:
   - Visual Studio 2022 (với C++ Desktop Development).
   - Python 3.10+ & Git.
   - EDK2 (TianoCore):
     ```bash
     git clone --recursive https://github.com/tianocore/edk2.git
     ```
2. **Cài đặt module vào EDK2**:
   - Sao chép thư mục `m1n6-EFI` vào `edk2/M1n6Pkg/` (hoặc khai báo trong `MdeModulePkg.dsc`).
3. **Build target**:
   ```bash
   build -p MdeModulePkg/MdeModulePkg.dsc -m m1n6-EFI/m1n6_efi.inf -a X64 -t VS2022 -b RELEASE
   ```
   Kết quả output: `Build/MdeModule/RELEASE_VS2022/X64/m1n6_efi.efi`.

## 💾 Cài Đặt Vào Hệ Thống (ESP Partition)
Module C# `EfiInstaller.cs` trong `SecHex-GUI` sẽ tự động:
1. Gán ký tự ổ đĩa cho phân vùng ESP (`mountvol S: /s`).
2. Chép binary vào `S:\EFI\Boot\bootx64.efi`.
3. Cập nhật BCD Entry qua `bcdedit`.
