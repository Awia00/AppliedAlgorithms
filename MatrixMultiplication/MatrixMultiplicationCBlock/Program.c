#include<stdio.h>
#include <stdlib.h>

int min(int a, int b) {
    return a < b ? a : b;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Call with an integer argument");
        exit(-1);
    }
    int b;

    if (argc == 3) {
        b = atoi(argv[2]);
    } else {
	b = 32;
    }

    int N = atoi(argv[1]);

    double *A = (double *)malloc((3* N * N) * sizeof(double));
    double *B = A + (N * N);
    double *C = B + (N * N);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = 1 + i + j;
            B[i * N + j] = 1 + i * j;
            C[i * N + j] = 0.0;
        }
    }


    for(int i0=0; i0<N; i0+=b) {
        for(int j0=0; j0<N; j0+=b) {
            for(int k0=0; k0<N; k0+=b) {
                for(int i=i0; i< min(i0 + b, N); i++) {
                    for(int j=j0; j< min(j0 + b, N); j++) {
                        for(int k=k0; k< min(k0 + b,N); k++) {
                            C[i * N + j] += A[i * N + k] * B[k * N + j];
                        }
                    }
                }
            }
        }
    }

    double sum = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sum += C[i * N + j];
        }
    }

    printf("%lf", sum);
    free(A);
}
