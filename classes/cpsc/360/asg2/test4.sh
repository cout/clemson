#!/bin/sh
./cp360 tmpfile outfile 4096
hdparm -f /dev/md0
