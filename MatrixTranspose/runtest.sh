#!/bin/bash
for i in {1..16}
do
	VAL=$((2**i))
	/usr/bin/time --format "%C\t%e\t%U\t%S" -o testresult.txt --append ./tr_sk $VAL $VAL 0
done
