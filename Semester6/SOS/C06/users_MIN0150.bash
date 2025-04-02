#!/bin/bash
for i in {1..50}; do
	useradd -m -s /bin/bash -p '' user$i
	# -m - Vytvoří home složku
	# -s - Nastaví výchozí shell
	# -p - Nastaví heslo
	passwd --expire user$i > /dev/null
	# --expire - nastaví heslo na expirované
	usermod -aG otherusers user$i
	# -a - appendne skupinu
	# -G - budu přidávat skupinu
	setquota -u user$i 666M 777M 0 0 /dev/sdb
	# -u - user
	# SOFT quota
	# HARD quota
	# SOFT inode
	# HARD inode
	echo "User user$i created!"
done
