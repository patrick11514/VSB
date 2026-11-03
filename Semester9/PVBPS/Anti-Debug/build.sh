#!/bin/bash
set -e

echo "[*] Building Windows 64-bit binary..."
x86_64-w64-mingw32-gcc -Wall -O2 -static main.c -lshell32 -o main.exe

echo "[+] Successfully built main.exe"
