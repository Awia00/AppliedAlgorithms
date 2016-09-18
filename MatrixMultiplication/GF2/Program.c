// Dense Matrix Matrix Multiplication over GF2 (AND - XOR)
// itu, course APALG
// by Riko Jacob
// first created Fall 16
// intended solution and specification of the problem

// On a Mac with MacPorts and gcc5
// gcc-mp-5 -fopenmp -O2 -Wall -mavx2 -Wa,-q -mfma -std=c11 binMultAll.c -lm && for i in `seq 1`; do OMP_NUM_THREADS=4 ./a.out $((512*12)) $i $(( 12 + $i)); done

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#include<math.h>
#include <immintrin.h>

typedef unsigned int myindex;
typedef __uint64_t myword; // make sure this one really has 64 bit

// the cheap pseudo random number generator
myword nextPR(myword x) { // compute a next pseudo-random number; the state is the number
  return (x+23446531L)*(x>> 5 );
};
myword seedPR(int xx) { // make a small number into something with reasonably distributed non-zero bits
  myword x = (myword) xx;
  x = x ^ (x<<27) ^ (x<<33) ;
  for(int i=0; i<10;i++) x = nextPR(x); //
  return x;
};
void rndBinMx(int N, int M, myword *A, int seed ) { // use the above to create a random GF2 (binary) matrix as bit array
  if( M % 64 ) exit(98); // required to store it in 64bit words
  myword x = seedPR( seed);
  for(int i=0; i< N*M /64; i++) {
    A[i] = x;
    x = nextPR(x);
  }
};

// some cheap but apparently good enough hash function
myword hash(myword a, myword b) { return ( (a + 9)  | (a<<27))*(b+2352351L);}
myword signBinMx(int N, int M, myword *A, int seed ) {
  myword x = seedPR( seed);
  for(int i=0; i< N*M/64; i++) {
    x = hash(x,A[i]);
  }
  return x;
};


// the definition of how the bits are interpreted as a matrix
int get(myword *A, int i, int m, int N, int M) { return 1L & (A[ i*(M/64) + m/64 ]>> (m & 63));}
void set(myword *A, int val, int i, int m, int N, int M) {
  //  printf("%d %d\n",i,m);
  A[ i*(M/64) + m/64 ] |= (1L << (m & 63));
  if(!val) {
    A[ i*(M/64) + m/64 ] ^= (1L << (m & 63));
  }
}

#define MIN(a,b) ((a) < (b) ? a : b)

void MxMBinNaive(int N, int M, int K, myword *A, myword *B, myword *C) {
  const int blockSize = 64;
  int i0,j0,k0,i,j,k;
  #pragma omp parallel for private(i,i0,j,j0,k,k0) shared(A, B,C, N,M,K)
  for(i0=0;i0<N;i0+=blockSize) {
    int ilim = MIN(i0 + blockSize, N);
    for(j0=0;j0<M;j0+=blockSize){
      int jlim = MIN(j0 + blockSize, M);
      for(k0=0;k0<K;k0+=blockSize){
        int klim = MIN(k0 + blockSize, K);
        for(i=i0; i< ilim; i++) {
          for(j=j0; j<jlim; j++) {
            int dp = get(C,i,j,N,M);
            for(k=k0; k<klim; k++) {
              if( get(A,i,k,N,K) && get(B,k,j, K,M) ) dp++;
            }
            set(C,dp&1,i,j, N,M);    // this is operating in GF2
            // set(C,dp,i,j, N,M);   // this would be boolean AND OR (on random matrices this is the all ones matrix with very very high prob)
          }
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  if( argc != 4 ){
    printf("usage: %s N seedA seedB\n",argv[0]);
    exit(2);
  }

  myindex N    = atoi(argv[1]);
  if( N & 255 ) N = (N|255)+1;

  myword seedA = atoi(argv[2]);
  myword seedB = atoi(argv[3]);

  // allocate some extra memory ...
  myindex size = N*N/8 + 520;
  myword *A = (myword *) malloc( size );
  myword *B = (myword *) malloc( size );
  myword *C = (myword *) malloc( size );
  // ... to increase the pointer to the next aligned position
  // we forget the original, because we do not intend to ever free the memory anyway
  A = (myword *) (((long long unsigned int) A | 255 ) +1 );
  B = (myword *) (((long long unsigned int) B | 255 ) +1 );
  C = (myword *) (((long long unsigned int) C | 255 ) +1 );

  // initialize the matrices in parallel
#pragma omp parallel sections shared(N,A,B,C,seedA,seedB) default(none)
  {
#pragma omp section
    {
      rndBinMx(N, N, A, seedA );
    }
#pragma omp section
    {
      rndBinMx(N, N, B, seedB );
    }
#pragma omp section
    {
      for(int i=0; i< N*N /128; i++) {
        C[i] = 0;
      }
    }
#pragma omp section
    {
      for(int i=N*N /128; i< N*N /64; i++) {
        C[i] = 0;
      }
    }
  }
  MxMBinNaive(N,N,N,A,B,C);
  myword sig = 1324123147L;
  for(int i=0;i<1;i++) sig = signBinMx(N,N,C,sig);
  printf("%d\n", (int) (sig & ((1<<15) -1)));

  return 0;
}
