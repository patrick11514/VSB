#!/bin/bash
mkdir $1
cd $1
cp ../ex_01.zip .
unzip ex_01.zip
cp -r ex_01/. ex_01/.vscode .
rm -r ex_01
rm ex_01.zip
cp ../../build.sh .
cp ../../test.sh .
cp ../Makefile .
