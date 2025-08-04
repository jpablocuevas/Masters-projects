# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "methods.h"

 // Pseudo-random number generator 
 
ld rand_num (int inf, int sup) {

	ld x;
	
	x = (ld) (rand () % (labs (sup-inf)) + inf) + ( (ld) rand ()) / pow (10,10);
	
	return x;
}

// Distance function. It returns the SQUARE of the distance between two points

ld dis2 (ld *x, size_t x_size) {

	int i;
	ld d = 0.;
	
	for (i = 0; i < x_size; i++) {
	
		d = d + pow (x[i],2);
	}
	
	return d;
}

// Function that updates the position of the particles from random numbers

ld *next_pos (ld *x_old, size_t x_size, ld alpha, int inf, int sup) {

	ld *x_new;
	
	x_new = (ld *) malloc (x_size * sizeof (ld));
	
	int i;
	
	for (i = 0; i < x_size; i++) {

		x_new [i] = x_old[i] + alpha*rand_num (inf, sup);
	}
	
	return x_new;
}

// Function that updates old coordinates to new ones

void update (ld *x_old, ld *x_new, size_t x_size) {
	
	int j;
	
	for (j = 0; j < x_size; j ++) {
		
			x_old[j] = x_new[j];
		}
}

// Function that checks whether or not the particle has exited the simulation cell

ld check (ld new, ld alpha) {

	if (pow (new, 2) >= pow (alpha, 2)) {
			
			if (new > 0) {
			
				return new - 2*alpha;
			}
			
			else {
			
				return 2*alpha + new;
			}	
		}
	
	return new;
}

// Memory allocation functions

ld **alloc_2d (int n, int m){

	ld **f;

	f  = (ld**) calloc (n, sizeof (ld*));
	
	int i;
	
	for (i = 0; i < n; i++){
	
		f[i] = (ld*) calloc (m, sizeof (ld));
	}
	
	return f;

}

void dealloc_2d (ld **f, int n){
	
	int i;
	
	for (i = 0; i < n; i++){
	
		free (f[i]);	
	}
	
	free (f);
}












