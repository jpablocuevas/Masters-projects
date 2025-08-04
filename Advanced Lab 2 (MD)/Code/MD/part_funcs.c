# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "part_funcs.h"


extern ld pi;

// Pseudo random number generator

ld rand_num (int inf, int sup) {
	
	return   (ld) (rand () % (labs (sup-inf) + 1) + inf) + (ld) rand () / RAND_MAX;
}

ld gauss_rand (void) {

	ld u1, u2, z1, z2;
	
	u1 = (ld) rand () / RAND_MAX;
	u2 = (ld) rand () / RAND_MAX;
	
	z1 = sqrt (-2*log (u1)) * cos (2*pi * u2);
	z2 = sqrt (-2*log (u1)) * sin (2*pi * u2);
	
	if (rand () % 2 == 0) {
		
		return z1;
	}
	
	else {
		
		return z2;
	}
	
	return 0.;
}

// Gives the distance between two points

ld dis (ld *x_1, ld *x_2, size_t size) {

	ld d = 0;
	
	for (int i = 0; i < size; i ++) {
	
		d = d + pow (* (x_1 + i) - *(x_2 + i), 2);
	}
	
	return sqrt (d);
}

// Memory allocation

ld **alloc_2D (size_t N, size_t M) {

	ld **arr = malloc (sizeof (ld *) * N);
	
	for (int i = 0; i < N; i ++) {
	
	
		*(arr + i) = malloc (sizeof (ld) * M);
	}
	
	return arr;
}

void dealloc_2D (ld **arr, size_t N) {

	for (int i = 0; i < N; i ++) {
	
		free (*(arr +i));
	}
	
	free (arr);
}

/*ld **alloc_3D (size_t N, size_t M) {

	ld ***arr = malloc (sizeof (ld *) * N);
	
	for (int i = 0; i < N; i ++) {
	
		*(arr + i) = malloc (sizeof (ld) * M);
		
	}
	
	return arr;
}

void dealloc_2D (ld **arr, size_t N) {

	for (int i = 0; i < N; i ++) {
	
		free (*(arr +i));
	}
	
	free (arr);
}
*/

// Regular lattice initialization 

void grid_init (int N, ld phi, ld sigma, int d) {
	
	FILE *pos;
	
	pos = fopen ("grid_part_init.txt", "w");
	
	int i;
	
	int n = 0; // Current particles in the grid
	
	ld L;
	
	if (d == 2) {
		
		L = sigma / 2 * sqrt (pi*N / phi);
		
		printf ("L = %Lf \n", L);
		
		ld dx = 0.05, dy = 0.05;
		
		ld sdx, sdy;
		
		sdx = dx;
		sdy = dy;
		
		while (n < N) {
		
			while (sdx < L / 2) {
			
				fprintf (pos, "%Lf \t %Lf \t %Lf \n", -L / 2 + sdx + sigma, -L / 2 + sdy + sigma, sigma);
				
				sdx = sdx + dx;
				
				n++;
			}
			
			sdx = 0;
			
			sdy = sdy + dy;

		}
		
	}
	
	else if (d == 3) {
		
		L = sigma / 2 *pow (4*pi*N / (3*phi), 1. / 3.);
		
		printf ("L = %Lf \n", L);
		
		ld dx = 0.1, dy = 0.1, dz = 0.1;
		
		ld sdx, sdy, sdz;
		
		sdx = dx;
		sdy = dy;
		sdz = dz;
		
		while (n < N) {
			
			while (sdy < L /2) {
			
				while (sdx < L / 2) {
				
					fprintf (pos, "%Lf \t %Lf \t %Lf\n", -L / 2 + sdx + sigma, -L / 2 + sdy + sigma, -L / 2 + sdz + sigma);
					
					sdx = sdx + dx;
					
					n++;
				}
				
				sdx = 0;
				
				sdy = sdy + dy;
			}
			
			sdy = 0;
			
			sdz = sdz + dz;
		}
				
	}
	
	fclose (pos);

}

// Random grid initialization 

void rand_init (int N, ld phi, ld sigma, int d) {

	int i, j;
	
	int acc; // Controls the acceptance of a new particle in the grid
	
	ld L, rc;
	
	FILE *pos = fopen ("rand_part_init.txt", "w");

	
	if (d == 2) {
		
		L = sigma / 2 * sqrt (pi*N / phi);
		
		printf ("%Lf \n", L);
		
		// Memory allocation
		
		ld **x = alloc_2D (N, d); // First index indicates the particle, second one the particle's coordinates
		
		int n = 1; // The number of current particles in existence 
		
		x[0][0] = rand_num ((-L + sigma) / 2, (L - sigma) / 2); // Initializing the first particle 
		x[0][1] = rand_num ((-L + sigma) / 2, (L - sigma) / 2);
		
		fprintf (pos, "%Lf \t %Lf \t %Lf \n", x[0][0], x[0][1], sigma);
		
		for (i = 1; i < N; i++) {
		
		acc = 0;
			
		while (acc == 0) {
		
			x[i][0] = rand_num ((-L + sigma) / 2, (L - sigma) / 2);
			x[i][1] = rand_num ((-L + sigma) / 2, (L - sigma) / 2);
		
			for (j = 0; j < n; j++) {
				
				acc = 1;
				
				rc = dis (x[i], x[j], d) - sigma; // Sigma is the particle's diameter
				
				if (rc <= 0) {
				
					acc = 0;
					
					break;
				}
				
			}
		}
		
		n ++; // Increases the number of accepted particles
			
		
			fprintf (pos, "%Lf \t %Lf \t %Lf \n", x[i][0], x[i][1], sigma);
		}
		
		dealloc_2D (x, N);
	}
	
	else if (d == 3) {
		
		L = sigma / 2 *pow (4*pi*N / (3*phi), 1. / 3.);
		
		printf ("%Lf \n", L);
		
		ld **x = alloc_2D (N, d); // First index stores the particle's number, second one its coordinates
		
		int n = 1; // The number of current particles in existence 
		
		x[0][0] = rand_num (-L / 2, L / 2); // Initializing the first particle 
		x[0][1] = rand_num (-L / 2, L / 2);
		x[0][2] = rand_num (-L / 2, L / 2);
		
		fprintf (pos, "%Lf \t %Lf \t %Lf \n", x[0][0], x[0][1], x[0][2]);
		
		for (i = 1; i < N; i++) {
		
		acc = 0;
			
		while (acc == 0) {
		
			x[i][0] = rand_num (-L / 2, L / 2);
			x[i][1] = rand_num (-L / 2, L / 2);
			x[i][2] = rand_num (-L / 2, L / 2);
		
			for (j = 0; j < n; j++) {
				
				acc = 1;
				
				rc = dis (x[i], x[j], d) - 2*sigma;
				
				if (rc == 0) {
				
					acc = 0;
					
					break;
				}		

			}
		
		}
			n ++; // Increases the number of accepted particles
		
			fprintf (pos, "%Lf \t %Lf \t %Lf \n", x[i][0], x[i][1], x[i][2]);
		}
		
		dealloc_2D (x, N);
	}
	
	fclose (pos);

}






