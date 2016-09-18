#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

typedef long int bigindex;
typedef long int smallindex;
typedef char content;
typedef __uint64_t myword; // make sure this one really has 64 bit

// the cheap pseudo random number generator
myword nextPR(myword x) { // compute a next pseudo-random number; the state is the number
  return (x+23446531L)*(x>> 5 );
};
myword seedPR(int xx) { // make a small number into something with reasonably distributed non-zero bits
  myword x = (myword) xx;
  x = x ^ (x<<27) ^ (x<<33) ;
  return x;
};
void rndMx(smallindex N, smallindex M, content *A, int seed ) { // use the above to create a random matrix ; needs some extra space
  myword *mA = (myword *) A;
  myword x = seedPR( seed);
  for(smallindex i=0; i<10;i++) x = nextPR(x);
  for(bigindex i=0; i<= N*(bigindex)M*sizeof(content)/sizeof(myword); i++) {
    mA[i] = x;
    x = nextPR(x);
  }
};

// some cheap but apparently good enough hash function
myword hash(myword a, myword b) { return ( (a + 9)  | (a<<27))*(b+2352351L);}
myword signMx(smallindex N, smallindex M, content *A, int seed ) { // A needs 8 content extra space for convinience
  for(bigindex i=0; i< 8;i++) A[N*(bigindex)M +i] = 0;
  myword *mA = (myword *) A;
  myword x = seedPR( seed);
  for(bigindex i=0; i<= N*(bigindex)M*sizeof(content)/sizeof(myword); i++) {
    x = hash(x,mA[i]);
  }
  return x;
};

// row major
bigindex rm(smallindex i, smallindex m, smallindex N, smallindex M) {
  return i*(bigindex)M + m; }
bigindex cm(smallindex i, smallindex m, smallindex N, smallindex M) {
  return m*(bigindex)N + i; }

#define MIN(a,b) ((a) < (b) ? a : b)

void transposeNaive(smallindex N, smallindex M,content *A, content *B) {
  const int blockSize = 32;
  smallindex i0, j0, i, j;
  #pragma omp parallel for shared(A,B,N,M) private(i0, j0, i, j) default(none)
  for (i0=0; i0<N; i0+=blockSize) {
    int limI = MIN(i0 + blockSize, N);
    for (j0=0; j0<M; j0+=blockSize) {
      int limJ = MIN(j0 + blockSize, M);
      for (i = i0; i < limI; i++) {
        for (j = j0; j < limJ; j++) {
          B[cm(i, j, N, M)] = A[rm(i, j, N, M)];
        }
      }
    }
  }
}

void  printMx(smallindex N,smallindex M,content *A) {
  for(smallindex i=0; i< N; i++) {
    for(smallindex j=0; j<M; j++)
      printf("%3d ", 128+A[rm(i,j,N,M)]);
    printf("\n");
  }
}


int main(int argc, char **argv) {
  if( argc != 4 ){
    printf("usage: %s N M seedA\n",argv[0]);
    exit(2);
  }

  bigindex N    = atoi(argv[1]);
  bigindex M    = atoi(argv[2]);
  myword seedA = atoi(argv[3]);

  // allocate some extra memory ...
  bigindex size = N*(bigindex)M * sizeof(content) + 8; // + 520;
  content *A = (content *) malloc( size );
  content *B = (content *) malloc( size );
  if( A==0 || B==0 ) exit(3);

  rndMx(N, M, A, seedA );
  if( N+M < 10) {
    printMx(N,M,A);
    printf("\n");
  }
  transposeNaive(N,M,A,B);
  if( N+M < 10)
    printMx(M,N,B);
  myword sig = 1324123147L;
  sig = signMx(N,M,B,sig);

  printf("%d\n", (int) (sig & ((1<<15) -1)));

  return 0;
}
