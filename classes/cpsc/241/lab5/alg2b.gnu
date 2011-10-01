set title "Curve fitting of O(n*n) to algorithm 2"
set xlabel "size of data"
set ylabel "time"
set yrange [0:35]
set terminal postscript
set output "alg2b.ps"
plot 'alg2b.dat', x*x*0.000000037
