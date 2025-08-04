# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include "methods.h"


int main (void) {
	
	time_t seed = 284820;

	srand ((unsigned) time (&seed));
	
	// Definition of parameters for the simulation
	
	long int i, j, inf = -1, sup = 1; // Random number range
	long int n = 100, N = 1; // Number of cycles, number of particles per cell 
	
	ld alpha = 1; // Maximum displacement
	
	// Box size
	
	ld Lx = 2.*alpha, Ly = 2*alpha, Lz = 2.*alpha; 
	ld box_size[3] = {Lx, Ly, Lz};
	
	// Position arrays
	
	size_t x_size = 3;
	
	ld **x_old;
	ld **x_new;
	
	x_old = (ld **) alloc_2d (N, x_size);
	x_new = (ld **) alloc_2d (N, x_size);
	
	// File writing
	
	FILE *pos;
	
	pos = fopen("positions.txt", "w");
	
	fputs ("x \t y \t z \t \n", pos);
	
	//fputs ("x \t y \t \n", pos);
	
	
	fprintf (pos, "%Lf \t %Lf \t %Lf \t \n", x_old[0][0], x_old[0][1], x_old[0][2]);
	
	//fprintf (pos, "%Lf \t %Lf \t \n", x_old[0][0], x_old[0][1]);
	
	// Main loop 
	
	for (i = 0; i < n; i++) {
	
		x_new[0] = (ld *) next_pos (x_old[0], x_size, alpha, inf, sup);
		
		for (j = 0; j < x_size; j++) {
			
			x_new[0][j] = (ld) check (x_new[0][j], alpha);
		}
		
		update (x_old[0], x_new[0], x_size);
		
		//fprintf (pos, "%Lf \t %Lf \t \n", x_new[0][0], x_new[0][1]);
		
		fprintf (pos, "%Lf \t %Lf \t %Lf \t \n", x_old[0][0], x_old[0][1], x_old[0][2]);
	}
	
	fclose (pos);
	
	dealloc_2d (x_old, N);
	dealloc_2d (x_new, N);
	
	
	return 0;
}

