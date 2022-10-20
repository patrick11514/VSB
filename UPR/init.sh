#!/bin/bash
mkdir $1
cd $1
wget https://github.com/geordi/upr-course/raw/master/assets/exercises/templates/ex_01.zip
unzip ex_01.zip
cp -r ex_01/. ex_01/.vscode .
rm -r ex_01
rm ex_01.zip
