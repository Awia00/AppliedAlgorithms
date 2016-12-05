set terminal png size 1920, 1080
set key outside
set title "Sorting"
set xlabel "Size"
set ylabel "Time/Size"
#set yrange [0:0.000003]
set xrange [1023:8388609]
set logscale

set output 'plot.png'

plot "time-random-pq.txt" using 1:3 title 'pq user', \
     "time-random-sort.txt" using 1:3 title 'std::sort user'
