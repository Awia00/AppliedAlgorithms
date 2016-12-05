set terminal png size 800,600
set key outside
set title "Sorting"
set xlabel "Size"
set ylabel "Time"
#set yrange [0:0.000003]
set xrange [0:1100000]
#set logscale

set output 'plot_time_lines.png'

plot "time-random-pq.txt" using 1:2 title 'pq' with lines, \
     "time-random-sort.txt" using 1:2 title 'std::sort' with lines

set ylabel "Time/Size"
set output 'plot_per_time_lines.png'

plot "time-random-pq.txt" using 1:($2/$1) title 'pq' with lines, \
     "time-random-sort.txt" using 1:($2/$1) title 'std::sort' with lines


