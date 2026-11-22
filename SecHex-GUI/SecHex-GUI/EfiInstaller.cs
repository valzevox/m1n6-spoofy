/*
 * m1n6-spoofy / EfiInstaller.cs
 * Language: C# (.NET 6.0-windows)
 * Description: Manages ESP partition mounting, EFI bootloader deployment, and BCD configuration
 */

using System;
using System.Diagnostics;
using System.IO;

namespace SecHex_GUI
{
    public static class EfiInstaller
    {
        private const char EspDriveLetter = 'S';

        /// <summary>
        /// Mounts the Windows EFI System Partition (ESP) to drive letter S:
        /// </summary>
        public static bool MountEsp()
        {
            try
            {
                RunCommand("mountvol", $"{EspDriveLetter}: /s");
                return Directory.Exists($"{EspDriveLetter}:\\EFI");
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// Unmounts the EFI System Partition
        /// </summary>
        public static void UnmountEsp()
        {
            try
            {
                RunCommand("mountvol", $"{EspDriveLetter}: /d");
            }
            catch
            {
            }
        }

        /// <summary>
        /// Deploys m1n6-EFI bootloader to the ESP partition
        /// </summary>
        public static bool DeployEfiBootloader(string sourceEfiPath, out string errorMessage)
        {
            errorMessage = string.Empty;

            if (!File.Exists(sourceEfiPath))
            {
                errorMessage = "Source EFI binary does not exist.";
                return false;
            }

            if (!MountEsp())
            {
                errorMessage = "Failed to mount EFI System Partition (Administrator rights required).";
                return false;
            }

            try
            {
                string targetDir = $"{EspDriveLetter}:\\EFI\\m1n6";
                if (!Directory.Exists(targetDir))
                {
                    Directory.CreateDirectory(targetDir);
                }

                string targetFile = Path.Combine(targetDir, "m1n6_efi.efi");
                File.Copy(sourceEfiPath, targetFile, true);

                // Configure Windows BCD to add / load m1n6-EFI boot entry
                RunCommand("bcdedit", $"/set {{bootmgr}} path \\EFI\\m1n6\\m1n6_efi.efi");

                return true;
            }
            catch (Exception ex)
            {
                errorMessage = ex.Message;
                return false;
            }
            finally
            {
                UnmountEsp();
            }
        }

        private static void RunCommand(string command, string arguments)
        {
            var psi = new ProcessStartInfo(command, arguments)
            {
                CreateNoWindow = true,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true
            };

            using var process = Process.Start(psi);
            process?.WaitForExit(10000);
        }
    }
}
