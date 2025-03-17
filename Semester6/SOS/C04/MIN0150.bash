#!/bin/bash
for folder in $(ls /home); do
	#                                 odebrani . a ..
	count=$(($(ls -a /home/$folder | wc -l) - 2))
	if [ "$count" -ge 100 ]; then
		touch /home/$folder/"BYL PREKROCEN MAX POCET SOUBORU VE SLOZCE"
	fi
done
