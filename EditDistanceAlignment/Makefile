default: ed edN

ed: edHirch.c
	cc -DCODEJUDGE -fopenmp -Wall -O2 -o ed edHirch.c

edN: edNaive.c
	cc -Wall -O2 -o edN edNaive.c

combined: program.c
	cc -DCODEJUDGE -fopenmp -Wall -O2 -o program program.c

clean: ed edN
	rm -f ed edN
