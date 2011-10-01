set title "Data Size vs. Time for Various Algorithms"
set xlabel "size of data"
set ylabel "time"
set yrange [0:35]
set terminal postscript
set output "algs.ps"
plot 'alg1.dat', 'alg2.dat', 'alg3.dat', 'alg4.dat'
