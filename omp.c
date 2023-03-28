#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define M 1000
#define N 1000
#define K 1000

int A[M][K], B[K][N], C[M][N];

int main() {
    // initialize matrices A and B with random values
    srand(time(NULL));
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            A[i][j] = rand() % 10;
        }
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = rand() % 10;
        }
    }

    // perform matrix multiplication in parallel
    clock_t start = clock();
    int T = omp_get_max_threads();
    #pragma omp parallel num_threads(T)
    {
        int id = omp_get_thread_num();
        int start = id * M / T;
        int end = (id + 1) * M / T;
        for (int i = start; i < end; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < K; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
    clock_t end = clock();
    double par_time = (double)(end - start) / CLOCKS_PER_SEC;

    // print resulting matrix C
    // for (int i = 0; i < M; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%d ", C[i][j]);
    //     }
    //     printf("\n");
    // }

    // performance metrics
    start = clock();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end = clock();
    double seq_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Sequential time: %f seconds\n", seq_time);
    printf("Parallel time: %f seconds\n", par_time);
    printf("Speedup factor: %f\n", seq_time / par_time);
    printf("Efficiency: %f%%\n", (seq_time / par_time) / T * 100);

    return 0;
}
