## Plan: Add factory save files

Move the factory game from serial-only persistence to file-backed save/load, ideally using a fixed path like `/factory_save.txt` on the mounted FAT16 filesystem. The recommended direction is to expose a small file I/O API through `os_api_t`, then let the app serialize its current state into a buffer and ask the kernel to read/write the file. Keep the game logic in `apps/` and keep the kernel changes limited to thin wrappers over the existing FAT16 layer.

**Steps**
1. Audit the current factory persistence path and separate concerns into three buckets: input serialization in the app, filesystem access in the kernel, and any remaining direct hardware/runtime dependencies. This establishes the exact boundary before adding API surface.
2. Extend `lib/api.h` with file-oriented helpers for the app, most likely `write_file(path, buffer, size)` and `read_file(path, buffer, max_size)` or an equivalent pair that returns the loaded size. Keep the API narrow and path-based so the app does not need FAT16 knowledge. *depends on 1*
3. Wire the new file helpers into the `kernel_api` initializer in `cli.c`. The CLI already owns the mounted filesystem state, so these helpers should delegate to the existing FAT16 code instead of duplicating filesystem logic in the app. *depends on 2*
4. Refactor `apps/factory_io.c` so save/load operate on an in-memory buffer and call the new API with `/factory_save.txt` as the target path. Preserve the current save format unless there is a strong reason to change it, because that minimizes the amount of parsing logic that must move.
5. Update `apps/factory.c` and `apps/factory.h` only as needed to thread the `os_api_t *` pointer through the app-side helpers and entry point. Keep the raw scancode polling change from the earlier migration intact.
6. Add any kernel-side helper functions needed to make file access safe and reusable. The likely implementation is a pair of small wrappers around the mounted global FAT state, not a new generic VFS layer.
7. Update app build wiring only if the new entry signature or shared API pointer requires it. The goal is for `factory.bin` to build cleanly with the new persistence path and no serial-only dependency for saving/loading.

**Relevant files**
- `/home/patrick115/Projects/VSB/Semester8/IvOS/apps/factory_io.c` — current save/load serialization code to redirect to file storage.
- `/home/patrick115/Projects/VSB/Semester8/IvOS/apps/factory.c` — app entry point and shared API pointer usage.
- `/home/patrick115/Projects/VSB/Semester8/IvOS/apps/factory.h` — app declarations and shared state.
- `/home/patrick115/Projects/VSB/Semester8/IvOS/lib/api.h` — file I/O API surface to add.
- `/home/patrick115/Projects/VSB/Semester8/IvOS/cli.c` — kernel-side API wiring and FAT-backed implementation hooks.
- `/home/patrick115/Projects/VSB/Semester8/IvOS/fs_state.h` — mounted filesystem state that the kernel wrappers should reuse.
- `/home/patrick115/Projects/VSB/Semester8/IvOS/drivers/fat_fs.h` and `/home/patrick115/Projects/VSB/Semester8/IvOS/drivers/fat_fs.c` — existing FAT16 read/write primitives to wrap.
- `/home/patrick115/Projects/VSB/Semester8/IvOS/apps/hello.c` — reference for the app API entry pattern.

**Verification**
1. Build the kernel/runtime with `make link` to confirm the new file API compiles and links against the mounted FAT16 state.
2. Rebuild the apps with `make apps` and confirm `factory.bin` still links after the persistence refactor.
3. Save and then reload a known game state from `/factory_save.txt`, checking that the restored map, inventory, and cursor state match the saved state.
4. Review the diff to verify the app no longer depends on serial-backed persistence for save/load and the kernel wrappers stayed thin.

**Decisions**
- Prefer path-based file helpers in `os_api_t` over exposing FAT16 internals to the app.
- Keep the save format stable unless you want a deliberate format change.
- Reuse the mounted global filesystem state already present in the CLI/kernel instead of introducing a second filesystem instance.

**Further Considerations**
1. If you want the save file to be human-readable, keep the current text format and write it to `/factory_save.txt`; if you want smaller saves, switch to a binary format after the API boundary is in place.
2. If you want the app to be able to choose filenames dynamically later, make the file helpers generic now; otherwise a fixed factory-specific path is the lowest-risk option.
