# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include "part_funcs.h"



ld pi = 3.14159265358979;

int main (void) {
	
	// Random see initialization
	
	time_t seed;

	srand ((unsigned) time (NULL));
	
	// Simuilation parameters
	
	int N = 40, d = 2;
	
	ld phi = 0.08, sigma = 0.1 ;
	
	// Grid initialization
	
	//rand_init (N, phi, sigma, d);
	
	grid_init (N, phi, sigma, d);

	return 0;
}
