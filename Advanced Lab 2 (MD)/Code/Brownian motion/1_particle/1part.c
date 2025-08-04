# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include "1part_funcs.h"


// Physical parameters

const ld T = 1., kB= 1., D = 1., pi = 3.1415926535897932;

// Simuilation parameters

ld ttot = 10, t = 0, dt = 0.01;

int main (void) {

	srand ( (unsigned) time (NULL));
	
	int i;
	size_t x_size = 2;
	
	// Memory allocation 
	
	ld *x_old = calloc (x_size, sizeof (ld));
	ld *x_new;
	
	// File management
	
	FILE *pos = fopen ("positions.txt", "w");
	
	if (x_size == 2) {
	
		fputs ("x \t y \t \n", pos);
	}
	
	else {
	
		fputs ("x \t y \t z \t \n", pos);
	}
	
	// Force pointer
	
	ld *F = calloc (sizeof (ld), x_size);
	
	// Main loop 
	
	while (t < ttot) {
	
	x_new = (ld *) move (x_old, x_size, F);
	
	if (x_size == 2) {
	
		fprintf (pos, "%Lf \t %Lf \t \n", x_new[0], x_new[1]);
	}
	
	else {
	
		fprintf (pos, "%Lf \t %Lf \t %Lf \t \n", x_new[0], x_new[1], x_new[2]);
	}
	
	update (x_new, x_old, x_size);
	
	t = t + dt;
		
	}
	
	fclose (pos);
	
	free (F);
	
	free (x_old);
	free (x_new);
	
	return 0;
}
