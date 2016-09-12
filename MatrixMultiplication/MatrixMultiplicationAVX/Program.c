// intended solution: naive matrix multiplication in C
// itu, course APALG
// by Riko Jacob
// first created Fall 16
 
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<x86intrin.h>

typedef double myfloat;
typedef unsigned int myindex;

#define MIN(a,b) ((a) < (b) ? a : b)

// always row, colum
// row major
myindex rm(int i, int m, int N, int M) {return i*M + m; }
// column major
myindex cm(int i, int m, int N, int M) {return m*N + i; }

void MxMnaive(int N, int M, int K, myfloat *A, myfloat *B, myfloat *C) {
  const int b = 32; // Should be optimum for a 32K size cache.

#pragma omp parallel for shared(A,B,C,N,M,K) default(none)
  for(int i0=0; i0<N; i0+=b) {
    for(int j0=0; j0<M; j0+=b) {
      for(int k0=0; k0<K; k0+=b) {
        for(int i=i0; i< i0 + b; i++) {
          for(int j=j0; j< j0 + b; j++) {
            __m256d c1 = _mm256_set_pd(0.0,0.0,0.0,0.0);
            for(int k=k0; k< k0 + b; k+=4) {
              __m256d a1 =  _mm256_loadu_pd(A + rm(i, k, N, K));
              __m256d b1 = _mm256_loadu_pd(B + cm(k, j, K, M));
              c1 = _mm256_mul_pd(a1, b1);

              double *res = (double*)&c1;

              C[rm(i,j, N,M)] += res[0] + res[1] + res[2] + res[3];
            }

          }
        }
      }
    }
  }
}
 
 
int x = 0;
int nextPR() {
  x =  (x+234532)*((x>> 5 )+12234);
  return x & 1023;
}
 
int hash(int a, int b) { return (a>>5 | a<<27)*(b+2352351);}
int main(int argc, char **argv){
 
  if( argc != 3 ) {
    printf("Usage: mult N seed\n");
    exit(1);
  }
 
  myindex N = atoi(argv[1]);
  x = atoi(argv[2]);
   
  myfloat *A = malloc( 3*N*N*sizeof(myfloat));
  myfloat *B = A + (N*N);
  myfloat *C = B + (N*N);
 
  int i,j;
 
  if( A == NULL || B==NULL || C==NULL ) {
    printf("Could not allocate memory");
    exit(2);
  }
 
  for(int k=0;k<N/2+1;k++) {
    for(i=0; i< N; i++) {
      for(j=0; j<N; j++) {
        A[rm(i,j,N,N)] = nextPR();
        B[cm(i,j,N,N)] = nextPR();
        C[rm(i,j,N,N)] = 0;
      }
    }
    MxMnaive(N,N,N, A,B,C);
  }
  int h = atoi(argv[2]);
  for(int k=0;k<3;k++)
    for(int i=0; i< N*N; i++) {
      //    printf("%f ", C[i]);
      h = hash(h, (int) C[i]);
    }
  printf( "%d\n", h & 1023);
  return 0;
}
