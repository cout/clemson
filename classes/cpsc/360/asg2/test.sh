#!/bin/sh
for x in 1 2 3 4 5 6 7 8 9; do
  hdparm -f /dev/md0
  time ./test2.sh
  hdparm -f /dev/md0
  time ./test3.sh
  hdparm -f /dev/md0
  time ./test4.sh
done;
