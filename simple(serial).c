#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 500
#define n 10
#define G 6.674e-11
#define delta_t 0.05
#include <string.h>
#include "time.h"
#include "math.h"
#define DIM 2
typedef double vect_t[DIM];


int q=0,i=0,k=0,X=0,Y=1, h=0;
	float x_diff, y_diff;
	float dist_cubed, dist;
	float pos[N][2];
	float old_pos[N][2];
	float vel[N][2];
	float mass[N];
	int step,part;
	double begin;
	double end;


int main(void){

	vect_t* forces = malloc(n*sizeof(vect_t));

	start = omp_get_wtime();
	for (step = 1; step <= n; step++) {

		forces = memset(forces, 0, n*sizeof(vect_t));

		//Initialize
		
		for(q=0,i=0;q<N;q++,i++) {

		     	pos[q][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
		     	pos[q][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;

		     	old_pos[i][X] = pos[q][X];
		     	old_pos[i][Y] = pos[q][Y];

		    // 	printf("Init Position %f, %f\n", pos[q][X], pos[q][Y]);

		     	vel[q][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
		     	vel[q][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;

		     //	printf("Init Velocity %f, %f\n", vel[q][X], vel[q][Y]);

		     	mass[q] = fabs((rand() / (double)(RAND_MAX)) * 2 - 1);
		     //	printf("Mass %f\n", mass[q]);

		 }
		 

		//Calculate Force
		
		
		for(q = 0; q < N ;q++) {

		    for(k = 0; k < N ;k++) {
			  if(k!=q){
				  x_diff = pos[q][X] - pos[k][X];
				  y_diff = pos[q][Y] - pos[k][Y];
				  dist = sqrt(x_diff*x_diff + y_diff*y_diff);
				  dist_cubed = dist*dist*dist;
				  forces[q][X] -= G*mass[q]*mass[k]/dist_cubed * x_diff;
				  forces[q][Y] -= G*mass[q]*mass[k]/dist_cubed * y_diff;
			  }
		   }
		}
	

		//Calculate New Position
		
		for(h=0 ;h < N; h++){
				pos[h][X] += delta_t*vel[h][X];
				pos[h][Y] += delta_t*vel[h][Y];
			//	 printf("Final position %f, %f\n", pos[h][X], pos[h][Y]);
				vel[h][X] += delta_t/mass[h]*forces[h][X];
				vel[h][Y] += delta_t/mass[h]*forces[h][Y];
			//	printf("Final Velocity %f, %f\n", vel[h][X], vel[h][Y]);

		  }

    }
	   
	    end = omp_get_wtime();
	   
		printf("The time is %f",end - begin);

		return EXIT_SUCCESS;
}
