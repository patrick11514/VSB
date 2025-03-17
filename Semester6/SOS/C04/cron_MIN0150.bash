#!/bin/bash
now=$(date +"%Y-%m-%d-%H.%M.%S")
file_name="$now-home.tar.gz"

if [ ! -d "/backups" ]; then
       mkdir /backups
fi

tar -czf /backups/$file_name /home 

