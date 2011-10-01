set title "Theoretical curves for Big-O"
set xlabel "size of data"
set ylabel "time"
set terminal postscript
set output "bigo.ps"
set xrange [0:10]
set yrange [0:1000]
plot x*x*x with lines, x*x with lines, x*log(x) with lines, x with lines
