#!/bin/sh
./xp2 -v 0 -t 1000000 < producer_consumer &
./xp2 -v 0 -t 1000000 < producer_consumer &
./xp2 -v 0 -t 1000000 < producer_consumer &
./xp2 -v 0 -t 1000000 -u < producer_consumer &
