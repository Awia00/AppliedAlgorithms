// Dense Matrix Matrix Multiplication over GF2 (AND - XOR)
// itu, course APALG
// by Riko Jacob
// first created Fall 16
// intended solution and specification of the problem

// On a Mac with MacPorts and gcc5
// gcc-mp-5 -fopenmp -O2 -Wall -mavx2 -Wa,-q -mfma -std=c11 binMultAll.c -lm && for i in `seq 1`; do OMP_NUM_THREADS=4 ./a.out $((512*12)) $i $(( 12 + $i)); done

#if NOAVX
#else
#define AVX 1
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

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
int get(myword *A, int row, int column, int N, int Columns) {
  return 1L & (A[ row*(Columns/64) + column/64 ]>> (column & 63));
}

void set(myword *A, int val, int row, int column, int N, int Columns) {
  //  printf("%d %d\n",i,m);
  A[ row*(Columns/64) + column/64 ] |= (1L << (column & 63));
  if(!val) {
    A[ row*(Columns/64) + column/64 ] ^= (1L << (column & 63));
  }
}

#define RM(i, m, N, M) (i)*(M)+(m)
#define CM(i, m, N, M) (m)*(N)+(i)
#define MIN(a,b) ((a) < (b) ? a : b)

void transposeNaive(int N, int M,myword *A, myword *B) {
  const int blockSize = 32;
  int i0, j0, i, j;
  #pragma omp parallel for shared(A,B,N,M) private(i0, j0, i, j) default(none)
  for (i0=0; i0<N; i0+=blockSize) {
    int limI = MIN(i0 + blockSize, N);
    for (j0=0; j0<M; j0+=blockSize) {
      int limJ = MIN(j0 + blockSize, M);
      for (i = i0; i < limI; i++) {
        for (j = j0; j < limJ; j++) {
          set(B, get(A, j, i, N, M), i, j, N, M);
//            B[CM(i, j, N, M)] = A[RM(i, j, N, M)];
        }
      }
    }
  }
}

#define MIN(a,b) ((a) < (b) ? a : b)

void MxMBinNaive(myindex N, myindex M, myindex K, myword *A, myword *B, myword *C) {
  const int blockSize = 32;
  A = __builtin_assume_aligned(A, 32);
  B = __builtin_assume_aligned(B, 32);
  C = __builtin_assume_aligned(C, 32);
  int i,i0,j,j0,k;
  myword *B1 = malloc(N*N/8 + 520);
  B1 = (myword *) (((long long unsigned int) B1 | 255 ) +1 );
  transposeNaive(K, M, B, B1);
  int klim = K/64;
  int mlim = M/64;
#if AVX
  __uint64_t cc[4];
  __m256i d;
  #pragma omp parallel for private(i,i0,j,j0,k, cc, d) shared(A,B,B1,C, N,M,K, mlim, klim) default(none)
#else
  #pragma omp parallel for private(i,i0,j,j0,k) shared(A,B,B1,C, N,M,K,mlim,klim) default(none)
#endif
  for(i0=0; i0<N; i0+=blockSize) {
    int ilim = MIN(i0+blockSize, N);
    for(j0=0; j0<M; j0+=blockSize) {
      int jlim = MIN(j0 + blockSize, M);
      for(i=i0; i<ilim; i++) {
        for(j=j0; j<jlim; j++) {
#if AVX
          d = _mm256_setzero_si256();
          for(k=0; k<klim; k+=4) {
            d = _mm256_xor_si256(d, _mm256_and_si256(_mm256_load_si256((__m256i*) (A + i*(klim)+k)), _mm256_load_si256((__m256i*) (B1 + j*(mlim)+k))));
          }
          _mm256_storeu_si256((__m256i*) cc, d);
          set(C,__builtin_popcountl(cc[0] ^ cc[1] ^cc[2] ^ cc[3]) & 1,i,j,N,M);
#else
          myword dp = 0;
          for(k=0; k<klim; k+=4) {
            dp ^= (A[i*(klim)+k] & B1[j*(mlim)+k]) ^ (A[i*(klim)+k+1] & B1[j*(mlim)+k+1]) ^ (A[i*(klim)+k+2] & B1[j*(mlim)+k+1]) ^ (A[i*(klim)+k+1] & B1[j*(mlim)+k+3]);
          }
          set(C,__builtin_popcountl(dp) & 1,i,j,N,M);
#endif
        }
      }
    }
  }
}

void MxMBinReference(myindex N, myindex M, myindex K, myword *A, myword *B, myword *C) {
  // No transpose
  // Use one word from A
  // And a column from B word by word
  // Then a loop that gets a bit from A-word, if the bit is one, do some XOR, else do nothing (which means multiply by nothing).
  // Layouts: A[N, K], B[K, M], C[N, M]
  for (int i = 0; i < N/64; i++) {
    for (int j = 0; j < M/64; j++) {
      for (int k = 0; k < K/64; k++) {
        if (get(A, i, k, N, K)) {
	  // The value is significant because the bit in a is 1
          C[i*(N/64)+j] ^= B[k*(K/64)+j];
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

  myword *A = (myword *) _mm_malloc( N*N/8, 32 );
  myword *B = (myword *) _mm_malloc( N*N/8, 32 );
  myword *C = (myword *) _mm_malloc( N*N/8, 32 );

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
      memset(C,0,N*N/64* sizeof C);
    }
  }
  MxMBinNaive(N,N,N,A,B,C);
//  MxMBinReference(N,N,N,A,B,C);
  myword sig = 1324123147L;
  for(int i=0;i<1;i++) sig = signBinMx(N,N,C,sig);
  printf("%d\n", (int) (sig & ((1<<15) -1)));

  return 0;
}
