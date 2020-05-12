#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

extern double mysecond();
extern double arrayMax();
extern void print_array();
extern int arrayMaxLoc();

int main()
{

    const int NTIMES = 100000000;
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

    srand(time(0)); // seed
    for (int i = 0; i < NTIMES; i++)
    {
        // Generate random number between 0 and 1
        x[i] = ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * 1000;
    }

    double maxval = 0.0;
    int maxloc = 0;
    double sum = 0.0;
    
    
    t = mysecond();
    #pragma omp parallel for schedule(static)
        for (int i = 0; i < NTIMES; i++){
            if (x[i] > maxval){
                maxval = x[i];
                maxloc = i;

                //printf("Thread %d is executing and maxval is %f \n", omp_get_thread_num(), maxval);
            }

            sum += x[i];

            
        }

    double timeTaken = mysecond() - t;

    double realSum = 0.0;
    for (int i = 0; i < NTIMES; i++){
        realSum += x[i];
    }

    double realMax = arrayMax(x, NTIMES);
    //print_array(x, NTIMES);
    printf("Total time taken is %f\n", timeTaken);
    printf("Real Max Val is %f\n", realMax);
    printf("Real Max Location is %d\n", arrayMaxLoc(x, NTIMES));
    printf("Real sum is %f\n", realSum);
    printf("Max Val using threads is %f\n", maxval);
    printf("Max Loc using threads is %d\n", maxloc);
    printf("Sum with threads is %f\n", sum);
    return 0;
}

double arrayMax(double y[], int num_of_elements)
{
    double max = 0.0;
    int maxlocation = 0;
    for (int j = 0; j < num_of_elements; j++)
    {
        if (y[j] > max){
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
        if (y[j] > max){
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
   for(i=0; i<num_elements; i++)
   {
	 printf("%f ", a[i]);
   }
   printf("\n");
}