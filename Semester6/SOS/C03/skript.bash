#!/bin/bash

## FIRST PART
for i in {1..1000}; do
    dd if=/dev/urandom of=$i bs=1 count=$((RANDOM)) &> /dev/null
    # dd -if - vstupní soubor
    #    -of - výsutpní soubor
    #    -bs - velikost bloku
    #    -count - počet bloků
done

ls -A . | wc -w; ls . | head

## SECOND PART

mkdir SOS_suda SOS_licha

for i in $(ls); do
    withoutDigit=$(echo "$i" | tr -d '0-9')
    # tr -d - odstraní znaky, tedy z názvu odebere číslice

    if [ -n "$withoutDigit" ]; then
    # -n string je nenulový, tedy v názvu souboru nejsou jen čislice a takový soubor chceme přeskočit 
        continue
    fi

    modulo=$((i % 2)) 
    if [ $modulo -eq 0 ]; then
    #  [ -eq ] - rovná se 
        mv $i SOS_suda
    else
        mv $i SOS_licha
    fi
done

printFolderInfo() {
    echo "Printing folder info for $1"
    echo "Count: $(ls $1 | wc -w)"
    #                      wc -w - spočítá počet slov 
    echo "First few files:"
    ls $1 | head
    echo "Last few files:"
    ls $1 | tail
}

printFolderInfo SOS_suda
printFolderInfo SOS_licha
