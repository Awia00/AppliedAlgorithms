// Dense Matrix Matrix Multiplication over GF2 (AND - XOR)
// itu, course APALG
// by Riko Jacob
// first created Fall 16
// intended solution and specification of the problem
 
// On a Mac with MacPorts and gcc5 
// gcc-mp-5 -fopenmp -O2 -Wall -mavx2 -Wa,-q -mfma -std=c11 binMultAll.c -lm && for i in `seq 1`; do OMP_NUM_THREADS=4 ./a.out $((512*12)) $i $(( 12 + $i)); done
 
#include<stdio.h>
#include<stdlib.h>
//#include<string.h>
 
//#include<math.h>
#include <sys/time.h>                // for gettimeofday()
#include <omp.h>
#include <immintrin.h>
 
typedef unsigned int myindex;
typedef __uint64_t myword; // make sure this one really has 64 bit
 
#define MIN(a,b) ((a) < (b) ? a : b)
#define GET(A,i,m,N,M) (1L & (A[ i*(M/64) + m/64 ]>> (m & 63)))
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
int get(myword *A, int i, int m, int D, int M) { return 1L & (A[ i*(M/64) + m/64 ]>> (m & 63));}

void set(myword *A, int val, int i, int m, int N, int M) {
  //  printf("%d %d\n",i,m);
  A[ i*(M/64) + m/64 ] |= (1L << (m & 63));
  if(!val) {
    A[ i*(M/64) + m/64 ] ^= (1L << (m & 63));
    if( get(A,i,m,N,M) ) {exit(7);} // just making sure...
  }
  else{
    if( ! get(A,i,m,N,M) ) exit(9);
  }
}

void transpose(int N, int M,myword *A, myword *B) {
  const int blockSize = 32;
  int i0, j0, i, j;
  #pragma omp parallel for shared(A,B,N,M) private(i0, j0, i, j) default(none)
  for (i0=0; i0<N; i0+=blockSize) {
    int limI = i0 + blockSize;
    for (j0=0; j0<M; j0+=blockSize) {
      int limJ = j0 + blockSize;
      for (i = i0; i < limI; i++) {
        for (j = j0; j < limJ; j++) {
          set(B, get(A, j, i, N, M), i, j, N, M);
        }
      }
    }
  }
}

void MxMBinBetter5(int N, int M, int K, myword *A, myword *B, myword *C, myword *Btrans) {
    transpose(N, M, B, Btrans);
    int i,j,k,dp;
    int const K64 = K/64;
    #pragma omp parallel for private(i,j,k,dp) shared(A, B,C, N,M,K)
    for(i=0; i< N; i++){
        for(j=0; j<M; j++) {
            __uint64_t cc[4];
            __m256i c =  _mm256_setzero_si256();
            for(k = 0; k<K64; k+=4)
            {
                __m256i a = _mm256_loadu_si256((__m256i *) &(A[ i*(K64) + k ]));
                __m256i b = _mm256_loadu_si256((__m256i *) &(Btrans[ j*(K64) + k ]));
                c = _mm256_xor_si256(_mm256_and_si256(a, b), c);
            }
            _mm256_storeu_si256((__m256i*) cc, c);
            dp = __builtin_popcountl(cc[0] ^ cc[1] ^ cc[2] ^ cc[3]);
            set(C,dp&1,i,j, N,M);  
        }
    }
}

void MxMBinBetter4(int N, int M, int K, myword *A, myword *B, myword *C, myword *Btrans) {
    transpose(N, M, B, Btrans);
    int i,j,k,dp;
    int const K64 = K/64;
    #pragma omp parallel for private(i,j,k,dp) shared(A, B,C, N,M,K)
    for(i=0; i< N; i++){
        for(j=0; j<M; j++) {
            dp = 0;
            for(k = 0; k<K64; k+=4)
            {
                __uint64_t cc[4];
                __m256i a = _mm256_loadu_si256((__m256i *) &(A[ i*(K64) + k ]));
                __m256i b = _mm256_loadu_si256((__m256i *) &(Btrans[ j*(K64) + k ]));
                __m256i c = _mm256_and_si256(a, b);

                
                _mm256_storeu_si256((__m256i*) cc, c);
                //_mm256_storeu_si256((__m256i*) cc, _mm256_and_si256(_mm256_loadu_si256((__m256i *) &(A[ i*(K64) + k ])), _mm256_loadu_si256((__m256i *) &(Btrans[ j*(K64) + k ]))));
                dp += __builtin_popcountl(cc[0]) + __builtin_popcountl(cc[1]) + __builtin_popcountl(cc[2]) + __builtin_popcountl(cc[3]);
            }
            set(C,dp&1,i,j, N,M);  
        }
    }
}

void MxMBinBetter3(int N, int M, int K, myword *A, myword *B, myword *C, myword *Btrans) {
    transpose(N, M, B, Btrans);
    int i,j,k,dp;  
    #pragma omp parallel for private(i,j,k,dp) shared(A, B,C, N,M,K)
    for(i=0; i< N; i++){
        for(j=0; j<M; j++) {
            dp = 0;
            for(k = 0; k<K; k+=64)
            {
                __uint64_t a = A[ i*(K/64) + k/64 ];
                __uint64_t b = Btrans[ j*(M/64) + k/64 ];
                __uint64_t c = a & b;
                dp += __builtin_popcountl(c);
            }
            set(C,dp&1,i,j, N,M);  
        }
    }
}

void MxMBinBetter2(int N, int M, int K, myword *A, myword *B, myword *C, myword *Btrans) {
    transpose(N, M, B, Btrans);
    int i,j,k, dp;  
    #pragma omp parallel for private(i,j,k,dp) shared(A, B,C, N,M,K)
    for(i=0; i< N; i++){
        for(j=0; j<M; j++) {
            dp = 0;
            for(k=0; k<K; k+=4) {
                if( GET(A,i,k,N,K) && GET(Btrans,j,k, K,M) ) dp++;
                if( GET(A,i,(k+1),N,K) && GET(Btrans,j,(k+1), K,M) ) dp++;
                if( GET(A,i,(k+2),N,K) && GET(Btrans,j,(k+2), K,M) ) dp++;
                if( GET(A,i,(k+3),N,K) && GET(Btrans,j,(k+3), K,M) ) dp++;
                
            }
            set(C,dp&1,i,j, N,M);  
        }
    }
}

void MxMBinBetter(int N, int M, int K, myword *A, myword *B, myword *C) {
    int i,j,k, i0,j0;
    int const b = 128;
    #pragma omp parallel for private(i,j,k,i0,j0) shared(A, B,C, N,M,K)
    for(i0 = 0; i0<N; i0+=b) {
        for(j0 = 0; j0<M; j0+=b) {
            for(i=i0; i<i0+b; i++) {
                for(j=j0; j<j0+b; j++) {
                    int dp = 0;
                    for(k=0; k<K; k++) 
                    {
                        if( get(A,i,k,N,K) && get(B,k,j, K,M) ) 
                        {
                            dp++;
                        }
                    }
                    set(C,dp&1,i,j, N,M);
                }
            }
        }
    }    
}

void MxMBinNaive(int N, int M, int K, myword *A, myword *B, myword *C) {
  int i,j,k;
  #pragma omp parallel for private(i,j,k) shared(A, B,C, N,M,K)
  for(i=0; i< N; i++)
    for(j=0; j<M; j++) {
      int dp = 0;
      for(k=0; k<K; k++) 
        if( get(A,i,k,N,K) && get(B,k,j, K,M) ) dp++;
      set(C,dp&1,i,j, N,M);    // this is operating in GF2
      // set(C,dp,i,j, N,M);   // this would be boolean AND OR (on random matrices this is the all ones matrix with very very high prob)
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
  myword *A =           _mm_malloc( size, sizeof *A);
  myword *B =           (myword *) malloc( size);
  myword *C =           _mm_malloc( size, sizeof *C );
  myword *Btrans =      _mm_malloc( size, sizeof *Btrans );
  // ... to increase the pointer to the next aligned position
  // we forget the original, because we do not intend to ever free the memory anyway 
  A = (myword *) (((long long unsigned int) A | 255 ) +1 );
  B = (myword *) (((long long unsigned int) B | 255 ) +1 );
  C = (myword *) (((long long unsigned int) C | 255 ) +1 );
 
  struct timeval t1, t2,t3,t4;//,t5,t6,t7;
  gettimeofday(&t1, NULL);
  // initialize the matrices in parallel
#pragma omp parallel sections shared(N,A,B,seedA,seedB) default(none)
  {
#pragma omp section
    {
      rndBinMx(N, N, A, seedA );
    }
#pragma omp section
    {
      rndBinMx(N, N, B, seedB );
    }
  }
  gettimeofday(&t2, NULL);
  MxMBinBetter5(N,N,N,A,B,C,Btrans);
  //MxMBinBetter4(N,N,N,A,B,C,Btrans);
  //MxMBinBetter3(N,N,N,A,B,C,Btrans);
  //MxMBinBetter2(N,N,N,A,B,C,Btrans);
  //MxMBinBetter(N,N,N,A,B,C);
  //MxMBinNaive(N,N,N,A,B,C);
  myword sig = 1324123147L;
  for(int i=0;i<1;i++) sig = signBinMx(N,N,C,sig);
  printf("%d\n", (int) (sig & ((1<<15) -1)));
   
  return 0;
}