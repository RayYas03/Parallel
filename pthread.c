#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 3
#define N 3
#define P 3
#define NUM_THREADS 4

int A[M][N], B[N][P], C[M][P];
pthread_mutex_t mutex;

void *matrix_multiply(void *thread_id) {
    intptr_t tid = (intptr_t) thread_id;
    int i, j, k, start, end, sum;
    start = (M/NUM_THREADS) * tid;
    end = (tid == NUM_THREADS-1) ? M : (M/NUM_THREADS)*(tid+1);
    for (i = start; i < end; i++) {
        for (j = 0; j < P; j++) {
            sum = 0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            pthread_mutex_lock(&mutex);
            C[i][j] = sum;
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

int main() {
    int i, j, rc;
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);
    // Initialize matrices A and B
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i*N + j;
            B[j][i] = i*N + j;
        }
    }
    // Create threads
    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, matrix_multiply, (void *)(intptr_t) i);
        if (rc) {
            printf("Error: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    // Wait for threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // Print result matrix C
    printf("Result matrix C:\n");
    for (i = 0; i < M; i++) {
        for (j = 0; j < P; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
