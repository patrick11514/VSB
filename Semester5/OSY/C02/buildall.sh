#!/bin/bash
cd STATIC && make
cd ../DYNAMIC && make
cd ../DYNAMIC2 && make

echo -e "\n\n\n"
echo "NORMAL LIB: export LD_LIBRARY_PATH=./DYNAMIC"
echo "HEX LIB: export LD_LIBRARY_PATH=./DYNAMIC2"