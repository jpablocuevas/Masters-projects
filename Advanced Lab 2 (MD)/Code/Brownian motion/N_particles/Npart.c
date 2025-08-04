# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include "funcs.h"


// Physical parameters

const ld T = 1., kB= 1., D = 1., pi = 3.1415926535897932;

// Simuilation parameters

ld ttot = 100, t = 0, dt = 0.1;

int N = 10; // Number of particles 

int main (void) {

	srand ( (unsigned) time (NULL));
	
	int i;
	size_t x_size = 2;
	
	// Memory allocation 

	ld **x_old = Nalloc (N, x_size);
	ld **x_new = Nalloc (N, x_size);

	
	// File management
	
	FILE *pos;
	
	char *name;
	
	/*if (x_size == 2) {
	
		fputs ("x \t y \t \n", pos);
	}
	
	else {
	
		fputs ("x \t y \t z \t \n", pos);
	}*/
	
	// Force pointer
	
	ld *F = calloc (sizeof (ld), x_size);
	
	// Main loop 
	
	while (t < ttot) {
		
				
		if ( (int) t % 5 == 0) {
				
			name = (char *) file_name ( (int) t);
			
			pos = fopen (name, "w");
		}
			
		for (i = 0; i < N; i++) {
			
			move (*(x_new +i), *(x_old + i), x_size, F);
			
			update (*(x_new + i), *(x_old + i), x_size);
			
					
			if ( (int) t % 5 == 0) {
			
				fprintf (pos, "%Lf \t %Lf \t \n", x_new[i][0], x_new[i][1]);
				
			}
		}
		
		if ( (int) t % 5 == 0) {
		
			fclose (pos);
			
			free (name);
		}	
	
		t = t + dt;
	}
	
	free (F);
	
	Ndealloc (N, x_old);
	Ndealloc (N, x_new);
	
	return 0;
}


/*		
	ld **x_old = Nalloc (N, x_size);
	ld **x_new = calloc (N, sizeof (ld *));

	
	// File management
	
	FILE *pos;
	
	char *name;
	
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
		
		for (i = 0; i < N; i++) {
			
			* (x_new + i) = (ld *) move (*(x_old + i), x_size, F);
			
			update (*(x_new + i), *(x_old + i), x_size);
			
		if ( (int) t % 5 == 0) {
			
			name = (char *) file_name ( (int) t);
		
			pos = fopen (name, "w");
		
			fprintf (pos, "%Lf \t %Lf \t \n", x_new[i][0], x_new[i][1]);
			
		}
		
		fclose (pos);
		
		free (name);
	}
	
		t = t + dt;
	}
	
	free (F);
	
	Ndealloc (N, x_old);
	Ndealloc (N, x_new);*/


/*

if (x_size == 2) {
	
		fprintf (pos, "%Lf \t %Lf \t \n", x_new[0], x_new[1]);
	}
	
	else {
	
		fprintf (pos, "%Lf \t %Lf \t %Lf \t \n", x_new[0], x_new[1], x_new[2]);
	}

*/
