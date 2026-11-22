/*
 * m1n6-spoofy / VolumeIdChanger.cs
 * Language: C# (.NET 6.0)
 * Description: Low-level NTFS & FAT32 Volume Serial Number changer
 */

using System;
using System.IO;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace SecHex_GUI
{
    public static class VolumeIdChanger
    {
        private const uint GENERIC_READ = 0x80000000;
        private const uint GENERIC_WRITE = 0x40000000;
        private const uint FILE_SHARE_READ = 0x00000001;
        private const uint FILE_SHARE_WRITE = 0x00000002;
        private const uint OPEN_EXISTING = 3;
        private const uint FSCTL_LOCK_VOLUME = 0x00090018;
        private const uint FSCTL_UNLOCK_VOLUME = 0x0009001C;

        [DllImport("kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        private static extern SafeFileHandle CreateFile(
            string lpFileName,
            uint dwDesiredAccess,
            uint dwShareMode,
            IntPtr lpSecurityAttributes,
            uint dwCreationDisposition,
            uint dwFlagsAndAttributes,
            IntPtr hTemplateFile);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool DeviceIoControl(
            SafeFileHandle hDevice,
            uint dwIoControlCode,
            IntPtr lpInBuffer,
            uint nInBufferSize,
            IntPtr lpOutBuffer,
            uint nOutBufferSize,
            out uint lpBytesReturned,
            IntPtr lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool ReadFile(
            SafeFileHandle hFile,
            byte[] lpBuffer,
            uint nNumberOfBytesToRead,
            out uint lpNumberOfBytesRead,
            IntPtr lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool WriteFile(
            SafeFileHandle hFile,
            byte[] lpBuffer,
            uint nNumberOfBytesToWrite,
            out uint lpNumberOfBytesWritten,
            IntPtr lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool SetFilePointer(
            SafeFileHandle hFile,
            int lDistanceToMove,
            out int lpDistanceToMoveHigh,
            uint dwMoveMethod);

        public static bool SpoofDriveVolumeId(char driveLetter, out string newSerial, out string error)
        {
            newSerial = string.Empty;
            error = string.Empty;

            string drivePath = $@"\\.\{char.ToUpper(driveLetter)}:";
            SafeFileHandle handle = CreateFile(
                drivePath,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                IntPtr.Zero,
                OPEN_EXISTING,
                0,
                IntPtr.Zero);

            if (handle.IsInvalid)
            {
                error = $"Cannot open volume {drivePath}. Administrator privileges required.";
                return false;
            }

            using (handle)
            {
                // Lock volume
                DeviceIoControl(handle, FSCTL_LOCK_VOLUME, IntPtr.Zero, 0, IntPtr.Zero, 0, out _, IntPtr.Zero);

                try
                {
                    byte[] sector = new byte[512];
                    if (!ReadFile(handle, sector, 512, out uint bytesRead, IntPtr.Zero) || bytesRead < 512)
                    {
                        error = "Failed to read boot sector.";
                        return false;
                    }

                    // Check NTFS OEM ID at offset 3: "NTFS    "
                    string oemId = System.Text.Encoding.ASCII.GetString(sector, 3, 8);
                    Random rnd = new Random();
                    byte[] newSerialBytes = new byte[8];
                    rnd.NextBytes(newSerialBytes);

                    if (oemId.StartsWith("NTFS"))
                    {
                        // NTFS Volume Serial Number is 8 bytes at offset 0x48
                        Buffer.BlockCopy(newSerialBytes, 0, sector, 0x48, 8);
                        newSerial = $"{newSerialBytes[7]:X2}{newSerialBytes[6]:X2}{newSerialBytes[5]:X2}{newSerialBytes[4]:X2}-{newSerialBytes[3]:X2}{newSerialBytes[2]:X2}{newSerialBytes[1]:X2}{newSerialBytes[0]:X2}";
                    }
                    else
                    {
                        // FAT32 Volume Serial Number is 4 bytes at offset 0x43
                        Buffer.BlockCopy(newSerialBytes, 0, sector, 0x43, 4);
                        newSerial = $"{newSerialBytes[3]:X2}{newSerialBytes[2]:X2}-{newSerialBytes[1]:X2}{newSerialBytes[0]:X2}";
                    }

                    // Rewind file pointer to beginning of volume
                    SetFilePointer(handle, 0, out _, 0);

                    if (!WriteFile(handle, sector, 512, out uint bytesWritten, IntPtr.Zero) || bytesWritten < 512)
                    {
                        error = "Failed to write modified boot sector to volume.";
                        return false;
                    }

                    return true;
                }
                finally
                {
                    // Unlock volume
                    DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, IntPtr.Zero, 0, IntPtr.Zero, 0, out _, IntPtr.Zero);
                }
            }
        }
    }
}
