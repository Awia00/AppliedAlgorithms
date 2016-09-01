#include<stdio.h>
#include <stdlib.h>

int main(int argv, char** args)
{
    if (argv < 2) {
        printf("Call with an integer argument");
        exit(-1);
    }

    int N = (int) strtol(args[1], (char**) NULL, 10);
    int M = N;
    int K = N;

    double *A = (double *)malloc(N * K * sizeof(double));
    double *B = (double *)malloc(K * M * sizeof(double));
    double *C = (double *)malloc(N * M * sizeof(double));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < K; j++) {
            A[i * N + j] = 1 + i + j;
        }
    }

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < M; j++) {
            B[i * K + j] = 1 + i * j;
        }
    }

    for(int i=0; i< N; i++)
        for(int j=0; j<M; j++) {
            C[i * N + j] = 0.0;
            for(int k=0; k<K; k++)
                C[i * N + j] += A[i * N + k] * B[k * K + j];
        }

    double sum = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            sum += C[i * N + j];
        }
    }

    printf("%lf", sum);
//    for(int i = 0; i < N; i++) free(A[i]);
//    for(int i = 0; i < K; i++) free(B[i]);
//    for(int i = 0; i < N; i++) free(C[i]);
//    free(A);
//    free(B);
//    free(C);

}
