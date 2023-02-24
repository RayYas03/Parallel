#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MAX_ITER 1000

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    // Set the size and resolution of the image
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double start_time=MPI_Wtime();
    int width = 800;
    int height = 600;
    double x_min = -2.0;
    double x_max = 1.0;
    double y_min = -1.5;
    double y_max = 1.5;

    // Allocate memory for the image
    unsigned char *image = (unsigned char*) malloc(width * height * sizeof(unsigned char));

    // Loop over each pixel in the image
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // Map the pixel coordinates to the complex plane
            double x = x_min + (x_max - x_min) * i / (double) width;
            double y = y_min + (y_max - y_min) * j / (double) height;

            // Perform the Mandelbrot iteration
            double zx = 0.0;
            double zy = 0.0;
            int iteration = 0;
            while (zx * zx + zy * zy < 4.0 && iteration < MAX_ITER) {
                double new_zx = zx * zx - zy * zy + x;
                double new_zy = 2.0 * zx * zy + y;
                zx = new_zx;
                zy = new_zy;
                iteration++;
            }

            // Set the pixel color based on the number of iterations
            if (iteration == MAX_ITER) {
                image[j * width + i] = 0; // black
            } else {
                double log_iter = log(iteration);
                double log_max_iter = log(MAX_ITER);
                double value = log_iter / log_max_iter;
                image[j * width + i] = (unsigned char) (255.0 * value); // grayscale
            }
        }
    }

    // Write the image data to a PGM file
    FILE *file = fopen("mandelbrot.pgm", "wb");
    fprintf(file, "P5\n%d %d\n255\n", width, height);
    fwrite(image, sizeof(unsigned char), width * height, file);
    fclose(file);

	double end_time = MPI_Wtime();
	double elapsed_time = end_time - start_time;
	printf("Elapsed time: %f seconds\n", elapsed_time);
	
    // Free the memory used by the image
    free(image);
    MPI_Finalize();
    return 0;
}
