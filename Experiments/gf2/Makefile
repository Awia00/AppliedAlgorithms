all: first

first: Program.c
	cc -O2 -Wall -mavx2 -Wa,-q -mfma -g -std=c99 -lm -fopenmp -o Program Program.c
