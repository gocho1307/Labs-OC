#!/usr/bin/gnuplot

set terminal pngcairo enhanced color size 28cm, 24cm
set output "plot312b.png"

set style line 1 lc rgb "#a3be8c" lt 1 lw 2; # green
set style line 2 lc rgb "#ebcb8b" lt 1 lw 2; # yellow
set style line 3 lc rgb "#5e81ac" lt 1 lw 2; # blue
set style line 4 lc rgb "#f8766d" lt 1 lw 2; # red
set style line 5 lc rgb "#8839ef" lt 1 lw 2; # purple

set border linewidth 0.5;
set xlabel "Strides (Bytes)";
set ylabel "Average Misses";
set grid;
set key title "Cache Size" right top;
set xr [0.75:1050000];
set yr [-0.05:5.5];
set format x "%.0f";

set xtics nomirror;
set ytics nomirror;

set logscale x 2;
plot "cm1_l2_plot.dat" using 1:2 with lines ls 1 title "64kB", \
     "cm1_l2_plot.dat" using 1:3 with lines ls 2 title "128kB", \
     "cm1_l2_plot.dat" using 1:4 with lines ls 3 title "256kB", \
     "cm1_l2_plot.dat" using 1:5 with lines ls 4 title "512kB", \
     "cm1_l2_plot.dat" using 1:6 with lines ls 5 title "1024kB";

pause mouse
