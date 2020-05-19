#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

extern double mysecond();

int main(){

    const int NTIMES = 1000000;
    double x[NTIMES];
    double t;
    
    

    srand(time(0)); // seed
    for(int i=0; i < NTIMES;i++){
        // Generate random number between 0 and 1
        x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
    }

    double maxval = 0.0; int maxloc = 0;
    t = mysecond();
    for (int i=0; i < NTIMES; i++){
        if (x[i] > maxval) 
            maxval = x[i]; maxloc = i;
    }

    double timeTaken = mysecond() - t;
    
    printf("Total time taken is %f\n", timeTaken);
    printf("Max Val is %f\n", maxval);

    return 0;
}

#include <sys/time.h>
double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}