# Tool Invocation & Anticheat Research Invariants

## Tool Schemas
- `run_command`: strictly pass `CommandLine`, `WaitMsBeforeAsync`, and `toolSummary`.
- `call_mcp_tool`: strictly pass PascalCase keys `ServerName`, `ToolName`, and `Arguments`.
- `write_file`: strictly pass `Path`, `Content`, and `Overwrite: true` when modifying existing files.

## Architectural Boundaries
- Keep the HWID Spoofer core clean: zero cheat, aimbot, or injection code in the spoofer codebase.
- Keep AI vision, recoil compensation, and overlay hijacks exclusively in dedicated documentation (`docs/architecture/`).

## Anti-Cheat Analysis Scope
- Break down multi-layer stacks into: Memory scanner, Integrity monitor, Telemetry collector.
- Trace account-linking vectors: Steam client registry, `loginusers.vdf`, Volume Serial Numbers (NTFS/FAT32), Network ARP/DNS.
- Distinguish hardware blacklist flags (permanent ban) from investigation watchlist flags (24h ban caused by account linking via Steam `loginusers.vdf` + volume serials + network ARP).

## UEFI Pre-OS Interception Invariants
- NVMe SN: Hook `EFI_DISK_INFO_PROTOCOL->Identify`, modify offset `0x04` (20 bytes ASCII).
- ATA SN: Modify offset `0x14` (20 bytes ASCII, byte-swapped pairs).
- SMBIOS: Always recompute `IntermediateChecksum` (offset `0x10..0x1E`) and `EntryPointStructureChecksum` (`0x00..Length`) after altering Type 1/2/3 tables to avoid integrity faults.
