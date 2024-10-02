#!/bin/bash

#build library
g++ -o libreader.so -shared -fPIC reader.cpp

#build source
g++ -o main main.cpp -L. -lreader

echo "export LD_LIBRARY_PATH=."