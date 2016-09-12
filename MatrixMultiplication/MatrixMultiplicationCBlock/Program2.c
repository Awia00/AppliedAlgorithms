// intended solution: naive matrix multiplication in C
// itu, course APALG
// by Riko Jacob
// first created Fall 16
 
#include<stdlib.h>
#include<stdio.h>
 
typedef double myfloat;
typedef unsigned int myindex;
 
// always row, colum
// row major
myindex rm(int i, int m, int N, int M) {return i*M + m; }
// column major
myindex cm(int i, int m, int N, int M) {return m*N + i; }
   
int min(int a, int b) {
    return a < b ? a : b;
}

void MxMnaive(int N, int M, int K, myfloat *A, myfloat *B, myfloat *C) {
  int b = 36; // Should be optimum for a 32K size cache.

  for(int i0=0; i0<N; i0+=b) {
    for(int j0=0; j0<M; j0+=b) {
      for(int k0=0; k0<K; k0+=b) {
        for(int i=i0; i< min(i0 + b, N); i++) {
          for(int j=j0; j< min(j0 + b, M); j++) {
            for(int k=k0; k< min(k0 + b,K); k++) {
              C[rm(i,j, N,M)] += A[rm(i,k, N,K)] * B[cm(k,j, K,M)];
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
