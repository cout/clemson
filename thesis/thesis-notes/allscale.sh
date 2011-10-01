#!/bin/sh
files=`ls *.jpg`

for f in $files; do
    ./onescale.sh $f
done
