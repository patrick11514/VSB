#!/bin/bash

rm -f ./*.tga
for letter in {A..Z}; do
    convert \
     -background black \
     -fill white  \
     -stroke black \
     -strokewidth 1 \
     -font impact.ttf \
     -pointsize 40 \
     -trim \
     -alpha off \
     -depth 8 \
     -type TrueColor \
     -define tga:image-origin=TopLeft \
     label:"$letter" \
     "$letter.tga"
done
