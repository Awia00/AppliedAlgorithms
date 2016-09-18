all: 1 2 naive

1: Program.c
	cc -std=c99 -Wall -O2 Program.c -o Program

2: Program2.c
	cc -std=c99 -fopenmp -Wall -O2 Program2.c -o Program2

naive: HashedNaive.c
	cc -std=c99 -fopenmp -Wall -O2 HashedNaive.c -o HashedNaive

clean: Program Program2 HashedNaive
	rm -f Program
	rm -f Program2
	rm -f HashedNaive
