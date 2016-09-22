default: Program.c
	cc -DNOAVX -O2 -Wall -g -std=c99 -lm -fopenmp -o Program Program.c
	cc -DNOAVX -O2 -Wall -g -std=c99 -lm -fopenmp -o Program1 Program1.c


avx: Program.c
	cc -O2 -Wall -mavx2 -Wa,-q -mfma -g -std=c99 -lm -fopenmp -o Program Program.c
	cc -O2 -Wall -mavx2 -Wa,-q -mfma -g -std=c99 -lm -fopenmp -o Program1 Program1.c
