#! /usr/bin/python

import os
from subprocess import Popen, PIPE

FNULL = open(os.devnull, 'w')

for method in ['pq', 'sort']:
    step = 12500
    size = 0
    while size < 1100001:
        print method, size
        for i in xrange(1,11):
            generator = Popen(["./generate.py", str(size)], stdout=PIPE)
            timer = Popen(["/usr/bin/time", "-a", "-f", "{}\t%e".format(size), "-o", "../results/time-random-{}.txt".format(method), "./program", method], stdin=generator.stdout, stdout=FNULL)
            generator.stdout.close()
            timer.wait()
        size += step


#./generate.py 120000 | time ./program $method > /dev/null            

#rm -f ../results/time-random-pq.txt ../results/time-random-sort.txt
#make
#for method in pq sort; do
#    ((step=50000))
#    for ((size = 0; size <= 1100000; size += step)); do
#        echo $method $size
#        ((ti=`date +%s`))
#        
#        for i in {1..10}; do
#            /usr/bin/time -a -f "$size\t%e\t%U\t%S" -o ../results/time-random-$method.txt ./generate.py $size | ./program $method > /dev/null
#        done
#        
#        ((ti2=`date +%s`))
#
#        ((spent=ti2-ti))
#
#        if (( spent / step >= 1 )); then
#            ((step = step * 2))
#            echo "$step"
#        fi
#    done
#done
