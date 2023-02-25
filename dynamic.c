#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#define WIDTH 1000
#define HEIGHT 1000
#define MAX_ITER 1000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    // Set the size and resolution of the image
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double start_time=MPI_Wtime();
    int i, j, iter, tid, num_threads;
    double x, y, x0, y0, xtemp, ytemp;
    int *image = (int *)malloc(WIDTH * HEIGHT * sizeof(int));

    #pragma omp parallel private(i, j, iter, tid, x, y, x0, y0, xtemp, ytemp)
    {
        tid = omp_get_thread_num();
        num_threads = omp_get_num_threads();

        #pragma omp for schedule(dynamic)
        for (i = 0; i < HEIGHT; i++) {
            for (j = tid; j < WIDTH; j += num_threads) {
                x0 = (double)(j - WIDTH / 2) / (WIDTH / 4);
                y0 = (double)(i - HEIGHT / 2) / (HEIGHT / 4);
                x = y = 0.0;
                iter = 0;
                while (x * x + y * y < 4 && iter < MAX_ITER) {
                    xtemp = x * x - y * y + x0;
                    ytemp = 2 * x * y + y0;
                    x = xtemp;
                    y = ytemp;
                    iter++;
                }
                image[i * WIDTH + j] = iter;
            }
        }
    }

    FILE *fp = fopen("mandelbrot.pgm", "wb");
    fprintf(fp, "P2\n%d %d\n%d\n", WIDTH, HEIGHT, MAX_ITER);
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            fprintf(fp, "%d ", image[i * WIDTH + j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %f seconds\n", elapsed_time);
    
    free(image);
    MPI_Finalize();
    
    return 0;
}
