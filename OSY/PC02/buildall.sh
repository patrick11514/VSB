#!/bin/bash
cd STATIC && ./build.sh
cd ../DYNAMIC && ./build.sh

echo "export LD_LIBRARY_PATH=./DYNAMIC"