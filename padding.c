#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef struct {double val; int loc; char pad[128];} tvals;

extern double mysecond();
extern double arrayMax();
extern void print_array();
extern int arrayMaxLoc();


int main()
{

    const int TOTAL_TRIALS = 5;

    int j = 0;

    const int NTIMES = 1000000;
    double x[NTIMES];
    double t;

    int k;
    int threadid;

    #ifdef _OPENMP
        k = 0;
    #pragma omp parallel
    #pragma omp atomic
        k++;
        printf("Number of Threads counted = %i\n", k);
    #endif

    while (j < TOTAL_TRIALS)
    {
        for (int i = 0; i < NTIMES; i++)
        {
            // Generate random number between 0 and 1
            x[i] = ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * 1000;
        }

        const int MAX_THREADS = omp_get_num_threads();

        double mval = 0.0;
        int mloc = 0;
        double sum = 0.0;
        int maxloc[MAX_THREADS];
        double maxval[MAX_THREADS];

        tvals maxinfo[MAX_THREADS];

        t = mysecond();

        #pragma omp parallel shared(maxinfo)
        {
            int id = omp_get_thread_num();
            maxinfo[id].val = -1.0e30;

            #pragma omp for
            for (int i = 0; i < NTIMES; i++){
                if(x[i] > maxinfo[id].val){
                    maxinfo[id].loc = i;
                    maxinfo[id].val = x[i];
                }
            }
        }


        mloc = maxinfo[0].loc;
        mval = maxinfo[0].val;

        for (int i = 0; i < MAX_THREADS; i++)
        {
            if (maxinfo[i].val > mval)
            {
                mval = maxinfo[i].val;
                mloc = maxinfo[i].loc;
            }
        }

        double timeTaken = mysecond() - t;

        printf("Total time taken is %f\n", timeTaken);

        j++;
    }

    srand(time(0)); // seed

    // double realMax = arrayMax(x, NTIMES);
    //print_array(x, NTIMES);
    // printf("Total time taken is %f\n", timeTaken);
    // printf("Real Max Val is %f\n", realMax);
    // printf("Real Max Location is %d\n", arrayMaxLoc(x, NTIMES));
    // printf("Max Val using threads is %f\n", mval);
    // printf("Max Loc using threads is %d\n", mloc);
    return 0;
}

double arrayMax(double y[], int num_of_elements)
{
    double max = 0.0;
    int maxlocation = 0;
    for (int j = 0; j < num_of_elements; j++)
    {
        if (y[j] > max)
        {
            max = y[j];
            maxlocation = j;
        }
    }

    return max;
}

int arrayMaxLoc(double y[], int num_of_elements)
{
    double max = 0.0;
    int maxlocation = 0;
    for (int j = 0; j < num_of_elements; j++)
    {
        if (y[j] > max)
        {
            max = y[j];
            maxlocation = j;
        }
    }

    return maxlocation;
}

#include <sys/time.h>
double mysecond()
{
    struct timeval tp;
    struct timezone tzp;
    int i;

    i = gettimeofday(&tp, &tzp);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

void print_array(double a[], int num_elements)
{
    int i;
    for (i = 0; i < num_elements; i++)
    {
        printf("%f ", a[i]);
    }
    printf("\n");
}