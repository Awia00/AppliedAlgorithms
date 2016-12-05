#! /bin/bash
rm -f ../results/time-random-pq.txt ../results/time-random-sort.txt
make
for method in pq sort; do
    for ((size = 65536; size < 131073; size += 8)); do
        for i in {1..10}; do
            if [ ! -f ../data/random-$size-$i.txt ]; then
                ./generate.py $size > ../data/random-$size-$i.txt
            fi
            echo $method $size $i
            /usr/bin/time -a -f "$size\t%e\t%U\t%S" -o ../results/time-random-$method.txt ./program $method < ../data/random-$size-$i.txt > /dev/null
        done
    done
done
make clean
