#!/bin/sh

FILES=`ls -1 tree3*.jpg`

for i in $FILES; do
  echo \<img src=\"$i\" width=100% height=100%\>\<br\>
done;
