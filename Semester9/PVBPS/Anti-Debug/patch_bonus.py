#!/usr/bin/env python3
import sys

def patch_binary(src="main.exe", dst="main_patched.exe"):
    try:
        with open(src, "rb") as f:
            data = bytearray(f.read())
    except FileNotFoundError:
        print(f"Error: {src} not found. Please run ./build.sh first.")
        sys.exit(1)

    # Pattern in main(): test bl, bl; jne 0x37; test sil, sil; jne 0x32
    # 84 db 75 37 40 84 f6 75 32
    pattern = b"\x84\xdb\x75\x37\x40\x84\xf6\x75\x32"
    idx = data.find(pattern)

    if idx == -1:
        print("Warning: Direct pattern not matched, searching for general jne sequence...")
        # fallback search
        import re
        m = re.search(b"\x84\xdb\x75.\x40\x84\xf6\x75.", data)
        if m:
            idx = m.start()
        else:
            print("Could not locate conditional jumps in main()!")
            sys.exit(1)

    print(f"[*] Found evasion jumps in main() at file offset: {hex(idx)}")
    # NOP out both jne jumps (0x90 0x90)
    data[idx+2:idx+4] = b"\x90\x90"
    data[idx+7:idx+9] = b"\x90\x90"

    with open(dst, "wb") as f_out:
        f_out.write(data)

    print(f"[+] Successfully generated patched binary: {dst}")
    print("[+] When executed under a debugger or VM, this binary will ignore detections and force malicious_way()!")

if __name__ == "__main__":
    patch_binary()
