# Tool Invocation & Anticheat Research Invariants

## Tool Schemas
- `run_command`: strictly pass `CommandLine`, `WaitMsBeforeAsync`, and `toolSummary`.
- `call_mcp_tool`: strictly pass PascalCase keys `ServerName`, `ToolName`, and `Arguments`.
- `write_file`: strictly pass `Path`, `Content`, and `Overwrite: true` when modifying existing files.

## Anti-Cheat Analysis Scope
- Break down multi-layer stacks into: Memory scanner, Integrity monitor, Telemetry collector.
- Trace account-linking vectors: Steam client registry, `loginusers.vdf`, Volume Serial Numbers (NTFS/FAT32), Network ARP/DNS.
- Distinguish hardware blacklist flags (permanent ban) from investigation watchlist flags (24h ban caused by account linking via Steam `loginusers.vdf` + volume serials + network ARP).
