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
    tstart = MPI_Wtime();

    //get total processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //get process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    srand(rank * SEED);

    int count= 0; 
    
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

    printf("MY count is %d\n", count);

    int total_count = 0;
    MPI_Win tt_count_win;
    MPI_Win_create(&total_count, sizeof(int), 1, MPI_INFO_NULL,MPI_COMM_WORLD, &tt_count_win);

    MPI_Win_fence( 0, tt_count_win);
    MPI_Accumulate(&count, 1, MPI_INT, 0, 0, 1, MPI_INT,MPI_SUM, tt_count_win);
    MPI_Win_fence(0, tt_count_win);


    if(rank == root){
        printf("Total count is %d\n", total_count);
        pi = ((double)total_count / (double)(proc_iter * size)) * 4.0;
    }

    tstop = MPI_Wtime();
    
    if(rank == root){
        printf("The result of pi is %f\n", pi);
        printf("Total elapsed time is %f\n", (tstop -tstart));
    }

    MPI_Win_free(&tt_count_win);
    MPI_Finalize();

    return 0;
}