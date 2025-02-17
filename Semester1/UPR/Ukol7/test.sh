#!/bin/bash
#test input on parameter $1
if [ -z "$1" ]; then
    ./main
else
    #check if folder exists
    if [ -d "$1" ]; then
        #check if file args exists in that folder   
        if [ -f "$1/args" ]; then
            #put args from file to args for program ./main
            ./main $(cat "$1/args") < ./$1/stdin > index.html
        else
            ./main < ./$1/stdin > index.html
        fi
    else
        echo "Folder does not exist"
    fi
fi