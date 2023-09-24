#!/usr/bin/gnuplot

set terminal pngcairo enhanced color size 28cm, 24cm
set output "plot311b.png"

set style line 1 lc rgb "#a3be8c" lt 1 lw 2; # green
set style line 2 lc rgb "#ebcb8b" lt 1 lw 2; # yellow
set style line 3 lc rgb "#5e81ac" lt 1 lw 2; # blue
set style line 4 lc rgb "#f8766d" lt 1 lw 2; # red

set style line 5 lc rgb "gray" lt 1 lw 1; # gray

set border linewidth 0.5;
set xlabel "Strides (Bytes)";
set ylabel "Average Misses";
set grid ls 5;
set key title "Cache Size" right top;
set xr [0.75:35000];
set yr [-0.05:1.8];

set xtics nomirror;
set ytics nomirror;

set logscale x 2;
plot "cm1_plot.dat" using 1:2 with lines ls 1 title "8kB",  \
     "cm1_plot.dat" using 1:3 with lines ls 2 title "16kB", \
     "cm1_plot.dat" using 1:4 with lines ls 3 title "32kB", \
     "cm1_plot.dat" using 1:5 with lines ls 4 title "64kB";

pause mouse
