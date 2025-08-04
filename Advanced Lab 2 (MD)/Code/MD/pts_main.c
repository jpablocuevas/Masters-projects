# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include "pts_funcs.h"



ld pi = 3.14159265358979;

int main (void) {

	time_t seed;

	
	srand ((unsigned) time (NULL));
	
	// Simuilation parameters
	
	int N = 1000, d = 3;
	
	ld rho = 0.5;
	
	// Grid initialization
	
	//rand_init (N, rho, d);
	
	grid_init (N, rho, d);
	

	return 0;
}
