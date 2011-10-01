set title "Curve fitting of O(n*log(n)) to algorithm 3"
set xlabel "size of data"
set ylabel "time"
set yrange [0:0.25]
set terminal postscript
set output "alg3b.ps"
plot 'alg3b.dat', x*log(x)*0.00000021
