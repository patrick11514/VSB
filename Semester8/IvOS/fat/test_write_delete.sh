#!/bin/bash
set -e

echo "=== Preparing test files ==="
echo "This is content for FILE1." > test_file1.txt

dd if=/dev/urandom of=test_file2.bin bs=1024 count=10 2>/dev/null
echo "Short file 3 data." > test_file3.txt
echo "File 4 replacing deleted space." > test_file4.txt

echo "=== Building project ==="
make

echo "=== Resetting FAT image ==="
make reset

echo "=== Iteration 1: Writing multiple files ==="
./fat write FILE1.TXT < test_file1.txt
./fat write FILE2.BIN < test_file2.bin
./fat write FILE3.TXT < test_file3.txt

echo "=== Current Filesystem state ==="
./fat print

echo "=== Iteration 2: Deleting a file (FILE2.BIN) ==="
./fat delete FILE2.BIN

echo "=== Current Filesystem state ==="
./fat print

echo "=== Iteration 3: Writing another file to reuse deleted space ==="
./fat write FILE4.TXT < test_file4.txt

echo "=== Final state of filesystem ==="
./fat print

fsck.vfat sd.img
