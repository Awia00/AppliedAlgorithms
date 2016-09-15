all: old new

old: Program.c
	cc -O2 -Wall -mavx2 -Wa,-q -mfma -g -std=c99 -lm -fopenmp -o Program Program.c

new: ProgramNew.c
	cc -O2 -Wall -mavx2 -Wa,-q -mfma -g -std=c99 -lm -fopenmp -o ProgramNew ProgramNew.c
