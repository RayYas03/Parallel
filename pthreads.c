#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define M 1000
#define N 1000
#define K 1000
#define T 4

int A[M][K];
int B[K][N];
int C[M][N];

void *multiply(void *arg) {
    int tid = *(int *)arg;
    int start = tid * (M / T);
    int end = (tid + 1) * (M / T);

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[T];
    int thread_ids[T];

    // initialize A and B matrices
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < K; j++) {
            A[i][j] = rand() % 100;
        }
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            B[i][j] = rand() % 100;
        }
    }

    // create threads
    for (int i = 0; i < T; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
    }

    // join threads
    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    // print resulting matrix C
    // for (int i = 0; i < M; i++) {
    //     for (int j = 0; j < N; j++) {
    //         printf("%d ", C[i][j]);
    //     }
    //     printf("\n");
    // }

    // performance metrics
    clock_t start = clock();
    for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < K; k++) {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
}
clock_t end = clock();
double seq_time = (double)(end - start) / CLOCKS_PER_SEC;

start = clock();
// create threads
for (int i = 0; i < T; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
}

// join threads
for (int i = 0; i < T; i++) {
    pthread_join(threads[i], NULL);
}
end = clock();
double par_time = (double)(end - start) / CLOCKS_PER_SEC;
printf("Sequential time: %f seconds\n", seq_time);
printf("Parallel time: %f seconds\n", par_time);
printf("Speedup factor: %f\n", seq_time / par_time);
printf("Efficiency: %f%%\n", (seq_time / par_time) / T * 100);

return 0;
}
   
