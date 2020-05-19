#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SEED 921
#define NUM_ITER 1000000000
int main(int argc, char *argv[])
{
    int count = 0;
    int rec = 0;
    double x, y, z, pi;
    int rank, size, i, provided;
    double start_time, end_time, binary;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    binary = log(size) / log(2);

    start_time = MPI_Wtime();
    srand(SEED * rank); // Important: Multiply SEED by "rank" when you introduce MPI!

    int proc_iter = ceil((NUM_ITER / size));

    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < proc_iter; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x * x) + (y * y));

        // Check if point is in unit circle
        if (z <= 1.0)
        {
            count++;
        }
    }

    for (int b = 1; b <= binary; b++)
    {
        int p = pow(2, b);
        if (rank % p == 0)
        {
            MPI_Recv(&rec, 1, MPI_INT, rank + pow(2, b - 1), MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int a = rank + pow(2, b - 1);
            count = count + rec;
        }
        else
        {
            int p1 = pow(2, b - 1);
            if (rank % p1 == 0)
            {
                MPI_Send(&count, 1, MPI_INT, (rank - p1), rank, MPI_COMM_WORLD);
            }
        }
    }

    if (rank == 0)
    {
        // Estimate Pi and display the result
        pi = ((double)count / (double)NUM_ITER) * 4.0;
        printf("The result is %f\n", pi);
    }

    end_time = MPI_Wtime();
    if (rank == 0)
    {
        printf("The time is %f\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}