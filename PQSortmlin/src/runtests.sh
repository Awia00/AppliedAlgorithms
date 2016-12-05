#! /bin/bash
rm -f ../results/time-random-pq.txt ../results/time-random-sort.txt
make
for method in pq sort; do
    for ((size = 9997500; size <= 10002500; size += 500)); do
        echo $method $size
        if [ ! -f ../data/random-$size.txt ]; then
            ./generate.py $size > ../data/random-$size.txt
        fi
        for i in {1..10}; do
            /usr/bin/time -a -f "$size\t%e\t%U\t%S" -o ../results/time-random-$method.txt ./program $method < ../data/random-$size.txt > /dev/null
        done
    done
done
make clean
