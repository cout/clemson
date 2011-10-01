#!/bin/sh
files=`ls *.ppm`

for f in $files; do
    echo Fixing $f
    ./fixppm $f tmp.ppm
    rm $f
    mv tmp.ppm $f
done
