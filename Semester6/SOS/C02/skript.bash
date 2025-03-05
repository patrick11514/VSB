#!/bin/bash

firstEnd() {
    ls -la . | head; echo "Konec první části skriptu."
}

secondEnd() {
    cat $(ls | head -n 1)/*.txt; cat $(ls | tail -n 1)/*.txt; echo "Konec druhé části skriptu."
}

mkdir $(wget -4 -O - http://seidl.cs.vsb.cz/download/seznam-obci-cr.txt 2> /dev/null | grep "ova" | grep -v "Nova" | tr " " "_")
# wget -4 připojuje se jen s ipv4
#      -O - outputuje data na stdout
# grep -v invertuje výběr

firstEnd

for folder in $(find . -mindepth 1 -type d -printf "%f\n"); do
# find -mindepth 1 hledá soubory pouze se zanořením 1 a více, tedy ignoruje se vždy startovní složka, v našem případě ./ 
#      -type d hledá pouze složky 
#      -printf vypíše na stdout text v zadaném formátu
    find . -mindepth 1 -type d -name "${folder:0:1}*" -printf "%f\n" > "./$folder/NAME.txt"
done

secondEnd
