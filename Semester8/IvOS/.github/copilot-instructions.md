# Copilot Instructions for IvOS

## Scope
These instructions apply to feature work in this custom 32-bit kernel project (bootloader + kernel + FAT16 + CLI).

## Core Development Rules
- Keep changes minimal and local. Avoid broad refactors unless explicitly requested.
- Prefer kernel-side helpers in `drivers/` over host-side code in `fat/` for runtime OS features.
- Validate every non-trivial change by building with `make link` from project root.
- Preserve freestanding constraints: no libc assumptions, no dynamic allocation from host runtime.

## Boot and Filesystem Know-How
- The prepared FAT16 volume is mounted at sector `2048`.
- For boot-time mount, use this order:
  1. `fat_init(&g_fat_fs)`
  2. configure/select partition start sector
  3. `fat_read_boot_sector(&g_fat_fs)`
- Keep mounted FS state global and shared (see `fs_state.h`) so CLI commands reuse one mounted instance.
- Default current directory cluster is root (`pwd_cluster = 0`).

## FAT16 Safety Rules
- Avoid copying `Fat16Entry` via direct struct assignment in hot paths; prefer pointers to `fs->current_file` or `memcpy`.
- Always skip non-usable directory entries:
  - `filename[0] == 0x00` -> end of directory
  - `filename[0] == 0xE5` -> deleted
  - `attributes == 0x0F` -> long-name metadata
  - `attributes & 0x08` -> volume label
- Bound directory iteration:
  - root directory: `boot_sector.root_dir_entries`
  - subdirectories: use safe upper bound and break on end markers
- Preserve and restore `current_file_index` when helper functions temporarily alter iteration state.

## CLI Feature Patterns
- Keep command handlers small (`do_<command>` helpers) and call them from `cli_loop()` dispatch.
- For path-based commands (`cd`, `cat`, `stat`, `tree`):
  - resolve path relative to `pwd_cluster` unless absolute path starts with `/`
  - support `.` and `..`
  - normalize prompt path string separately from FAT cluster state
- Prefer printing user-facing output to VGA (`vga_print`), not serial.

## Debugging Strategy
- Use serial for low-level diagnostics and crash localization.
- Add debug markers around suspicious operations, then remove them after fix.
- If a reboot occurs right after a function call returns, inspect caller-side stack/copy behavior first.
- Print both control-flow markers and key computed values (cluster, sector, offsets) during FAT debugging.

## Common Crash Triggers Observed
- Heavy local stack usage and large temporary arrays in nested CLI/FAT code paths.
- Risky struct copies of packed FAT entries in tight loops.
- Unbounded directory traversal when end-of-directory markers are not respected.
- Mixing debug channels (VGA vs serial) leading to false conclusions while diagnosing.

## String and Conversion Utilities
- Use project-local helpers from `lib/string.*` and `lib/convert.*`.
- Ensure destination buffers are sufficiently sized and null-terminated.
- Keep helper signatures simple and explicit (input value + output buffer).

## Change Checklist
- Build succeeds (`make link`).
- New command appears in `help` output.
- No leftover temporary debug spam in normal operation.
- FAT operations handle invalid/missing targets gracefully with user-readable errors.
