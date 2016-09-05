#include<stdio.h>
#include <stdlib.h>

int main(int argv, char** args)
{
    if (argv < 2) {
        printf("Call with an integer argument");
        exit(-1);
    }

    int N = atoi(args[1]);
    int M = N;
    int K = N;

    double *A = (double *)malloc((N * K + K * M + N * M) * sizeof(double));
    double *B = A + (N * K);
    double *C = B + (K * M);

    if (K != N) {
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
    } else {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i * N + j] = 1 + i + j;
                B[i * N + j] = 1 + i * j;
            }
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
    free(A);
}
