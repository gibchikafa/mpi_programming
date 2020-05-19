#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char *argv[]){
    int rank, size, provided;
    double tstart, tstop, pi;
    int root = 0;

    //initialize mpi
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Barrier(MPI_COMM_WORLD);
    tstart = MPI_Wtime();

    //get total processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //get process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    srand(rank * SEED);

    int count= 0; int total_count = 0;

    
    double x, y, z;


    int proc_iter = ceil((NUM_ITER/size));

    // Calculate PI following a Monte Carlo method
    for (int iter =  0; iter < proc_iter; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            count++;
        }
    }

    if(rank != root){
        //send count to process 0
        MPI_Send(&count, 1, MPI_INTEGER, 0, rank, MPI_COMM_WORLD);
    }
    else{
        total_count = count;
        for(int i = 1; i < size; i++){
            MPI_Recv(&count, 1, MPI_INTEGER, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count += count;
        }

        pi = ((double)total_count / (double)proc_iter * size) * 4.0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    tstop = MPI_Wtime();
    
    if(rank == 0){
        printf("The result of pi is %f\n", pi);
        printf("Total elapsed time is %f\n", (tstop -tstart));
    }

    MPI_Finalize();

    return 0;
}