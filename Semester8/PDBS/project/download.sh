#!/bin/bash

mkdir -p data

cd data

if ! [ -f NGC.csv ]; then
    echo "Downloading NGC.csv..."
    wget https://github.com/mattiaverga/OpenNGC/raw/refs/heads/master/database_files/NGC.csv
fi

if ! [ -f hyg_v42.csv ]; then
    echo "Downloading hyg_v42.csv..."
    curl https://codeberg.org/astronexus/hyg/media/branch/main/data/hyg/CURRENT/hyg_v42.csv.gz | gunzip > hyg_v42.csv
fi

if ! [ -f messier.csv ]; then
    echo "Downloading messier.csv..."
    wget https://raw.githubusercontent.com/lgbouma/obs/refs/heads/master/data/catalogs/messier.csv
fi
