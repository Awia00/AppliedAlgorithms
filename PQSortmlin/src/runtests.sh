#! /bin/bash
for method in pq sort; do
    echo $method > ../results/time-random-$method.txt
    for i in {999990..1000000}; do
        /usr/bin/time -a -f "$i\t%e\t%U\t%S" -o ../results/time-random-$method.txt ./program $method < ../data/random-$i.txt > /dev/null
    done
done
