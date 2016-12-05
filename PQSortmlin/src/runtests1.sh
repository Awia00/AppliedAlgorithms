#! /bin/bash
rm -f ../results/time-random-pq.txt ../results/time-random-sort.txt
make
for method in pq sort; do
    ((step=50000))
    for ((size = 0; size <= 1100000; size += step)); do
        echo $method $size
        ((ti=`date +%s`))
        
        for i in {1..10}; do
            /usr/bin/time -a -f "$size\t%e\t%U\t%S" -o ../results/time-random-$method.txt ./generate.py $size | ./program $method > /dev/null
        done
        
        ((ti2=`date +%s`))

        ((spent=ti2-ti))

        if (( spent / step >= 1 )); then
            ((step = step * 2))
            echo "$step"
        fi
    done
done
make clean
