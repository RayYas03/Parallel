#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_ITER 1000

int mandelbrot(double x, double y) {
    int i = 0;
    double a = 0, b = 0;
    while (i < MAX_ITER && a*a + b*b < 4) {
        double temp_a = a*a - b*b + x;
        b = 2*a*b + y;
        a = temp_a;
        i++;
    }
    return i;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    // Set the size and resolution of the image
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double start_time=MPI_Wtime();
    int pixels[WIDTH][HEIGHT];
    double xmin = -2, xmax = 1, ymin = -1.5, ymax = 1.5;
    double dx = (xmax - xmin) / WIDTH;
    double dy = (ymax - ymin) / HEIGHT;

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            double x = xmin + i*dx;
            double y = ymin + j*dy;
            pixels[i][j] = mandelbrot(x, y);
        }
    }

    // Write the output to a PGM file
    FILE *file = fopen("mandelbrot.pgm", "w");
    fprintf(file, "P2\n%d %d\n%d\n", WIDTH, HEIGHT, MAX_ITER-1);
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            fprintf(file, "%d ", pixels[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    printf("Elapsed time: %f seconds\n", elapsed_time);
    MPI_Finalize();

    return 0;
}
