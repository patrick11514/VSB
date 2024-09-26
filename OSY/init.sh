#!/bin/bash
mkdir -p $1
cp main.cpp $1
cp CMakeLists.txt $1
cd $1
sed -i "s/%PROJECT_NAME%/$1/g" CMakeLists.txt
