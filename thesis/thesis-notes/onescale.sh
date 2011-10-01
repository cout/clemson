#!/bin/sh
echo Scaling $1
convert $1 -geometry 50%x50% tmp.jpg
rm $1
mv tmp.jpg $1
