#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank of the current process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Print the rank of the current process
    printf("Hello world from process %d\n", rank);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
