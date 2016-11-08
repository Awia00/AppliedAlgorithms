#!/bin/bash
touch testresult.dat
echo "# This file is called testresult.dat" > testresult.dat
echo "# Side size\tReal\tUser\tSystem" >> testresult.dat
for i in {1..16384}
do
	/usr/bin/time --format "$i\t%e\t%U\t%S" -o testresult.dat --append ./tr_sk $i $i 0
done
