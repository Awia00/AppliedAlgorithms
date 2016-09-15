// intended solution: naive matrix multiplication in C
// itu, course APALG
// by Riko Jacob
// first created Fall 16
 
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<immintrin.h>
#include<omp.h>
 
typedef double myfloat;
typedef unsigned int myindex;
 
// always row, colum
// row major
myindex rm(int i, int m, int N, int M) {return i*M + m; }
// column major
myindex cm(int i, int m, int N, int M) {return m*N + i; }
   
void MxMnaive(int N, int M, int K, myfloat *A, myfloat *B, myfloat *C) {
  A = __builtin_assume_aligned(A, 32);
  B = __builtin_assume_aligned(B, 32);
  C = __builtin_assume_aligned(C, 32);
  const int b = 64;

#pragma omp parallel for shared(A,B,C,N,M,K) default(none)
  for(int i0=0; i0<N; i0+=b) {
    for(int j0=0; j0<M; j0+=b) {
      for(int k0=0; k0<K; k0+=b) {
        for(int i=i0; i< i0 + b; i++) {
          for(int j=j0; j< j0 + b; j++) {
            __m256d c1 = _mm256_setzero_pd();

              __m256d a1 = _mm256_load_pd(A + rm(i, k0, N, K));
              __m256d b1 = _mm256_load_pd(B + cm(k0, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+4, N, K));
              b1 = _mm256_load_pd(B + cm(k0+4, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+8, N, K));
              b1 = _mm256_load_pd(B + cm(k0+8, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+12, N, K));
              b1 = _mm256_load_pd(B + cm(k0+12, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+16, N, K));
              b1 = _mm256_load_pd(B + cm(k0+16, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+20, N, K));
              b1 = _mm256_load_pd(B + cm(k0+20, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+24, N, K));
              b1 = _mm256_load_pd(B + cm(k0+24, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+28, N, K));
              b1 = _mm256_load_pd(B + cm(k0+28, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);
              
              a1 = _mm256_load_pd(A + rm(i, k0+32, N, K));
              b1 = _mm256_load_pd(B + cm(k0+32, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+36, N, K));
              b1 = _mm256_load_pd(B + cm(k0+36, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+40, N, K));
              b1 = _mm256_load_pd(B + cm(k0+40, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+44, N, K));
              b1 = _mm256_load_pd(B + cm(k0+44, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+48, N, K));
              b1 = _mm256_load_pd(B + cm(k0+48, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+52, N, K));
              b1 = _mm256_load_pd(B + cm(k0+52, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+56, N, K));
              b1 = _mm256_load_pd(B + cm(k0+56, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);

              a1 = _mm256_load_pd(A + rm(i, k0+60, N, K));
              b1 = _mm256_load_pd(B + cm(k0+60, j, K, M));
              c1 = _mm256_fmadd_pd(a1, b1, c1);
            double *res = (double*)&c1;

            C[rm(i,j, N,M)] += res[0] + res[1] + res[2] + res[3];
          }
        }
      }
    }
  }
}
 
 
int x = 0;
int nextPR() {
  x =  (x+234532)*((x>> 5 )+12234);
  return x & 16383;
}
 
long int hash(long int a, long int b) { return (a  | a<<27)*(b+2352351);}
 
int main(int argc, char **argv){
 
  if( argc != 3 ) {
    printf("Usage: mult N seed\n");
    exit(1);
  }
 
  omp_set_num_threads(1);
   
  myindex N = atoi(argv[1]);
  x = atoi(argv[2]);
   
  myfloat *A = _mm_malloc(3*N*N*sizeof *A, 32);
  myfloat *B = A + (N*N);
  myfloat *C = B + (N*N);
 
  int i,j;
 
  if( A == NULL || B==NULL || C==NULL ) {
    printf("Could not allocate memory");
    exit(2);
  }
 
  for(i=0; i< N; i++) {
    for(j=0; j<N; j++) {
      A[rm(i,j,N,N)] = nextPR();
      B[cm(i,j,N,N)] = nextPR();
      C[rm(i,j,N,N)] = 0;
    }
  }
  MxMnaive(N,N,N, A,B,C);
 
  int h = atoi(argv[2]);
  for(int k=0;k<3;k++)
    for(int i=0; i< N*N; i++) {
      //    printf("%f ", C[i]);
      h = hash(h, (long int) C[i]);
    }
  printf( "%d\n", h & 1023);
  return 0;
}