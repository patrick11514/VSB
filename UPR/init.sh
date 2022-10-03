#!/bin/bash
cd $1
wget https://github.com/geordi/upr-course/raw/master/assets/exercises/templates/ex_01.zip
unzip ex_01.zip
cp -r $1/ex_01/* $1/
rm -rf $1/ex_01
rm -r $1/ex_01.zip
