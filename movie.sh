#!/usr/bin/env bash

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin 
export PATH

num=1; for i in $(ls | cut -d "." -f 1 | sort -n); do mv $i.png img"$(printf "%03d" $num)".png; num=$((num+1)); done

for i in *; do convert img036.png -resize 800x400 img036.png; done
ffmpeg -f image2 -r 1/0.5 -i img%03d.png -c:v libx264 -r 30 out.mp4
