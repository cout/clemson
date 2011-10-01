set title "Time vs. # of partitions"
set xlabel "# of partitions"
set ylabel "time"
set terminal postscript
set output "temp.ps"
plot 'temp1.dat' with lines, 'temp4.dat' with lines

