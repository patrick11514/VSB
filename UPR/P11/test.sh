#!/bin/bash
#test input on parameter $1
if [ -z "$1" ]; then
    ./main
else
    #check if folder exists
    if [ -d "$1" ]; then
        ./main < ./$1/stdin
    else
        echo "Folder does not exist"
    fi
fi