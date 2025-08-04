# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "pts_funcs.h"


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

// Regular lattice initialization 

void grid_init (int N, ld rho, int d) {
	
	FILE *pos;
	
	pos = fopen ("grid_pts_init.txt", "w");
	
	int i, j;
	
	int n = 0; // Current particles in the grid
	
	ld L;
	
	if (d == 2) {
		
		L = pow (N / rho, 1. / 2.);
		
		printf ("L = %Lf \n", L);
		
		ld dx = 1, dy = 1;
		
		ld sdx, sdy;
		
		sdx = dx;
		sdy = dy;
		
		while (n < N) {
		
			while (sdx < L / 2) {
			
				fprintf (pos, "%Lf \t %Lf \n", -L / 2 + sdx, -L / 2 + sdy);
				
				sdx = sdx + dx;
				
				n++;
			}
			
			sdx = 0;
			
			sdy = sdy + dy;
			
			printf ("%d \n", n);

		}
		
	}
	
	else if (d == 3) {
		
		L = pow (N / rho, 1. / 3.);
		
		printf ("L = %Lf \n", L);
		
		ld dx = 1, dy = 1, dz = 1;
		
		ld sdx, sdy, sdz;
		
		sdx = dx;
		sdy = dy;
		sdz = dz;
		
		while (n < N) {
			
			while (sdy < L /2) {
			
				while (sdx < L / 2) {
				
					fprintf (pos, "%Lf \t %Lf \t %Lf\n", -L / 2 + sdx, -L / 2 + sdy, -L / 2 + sdz);
					
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

void rand_init (int N, ld rho, int d) {

	int i;
	
	ld L;
	
	FILE *pos = fopen ("rand_pts_init.txt", "w");
	
	if (d == 2) {
		
		L = pow (N / rho, 1. / 2.);
		
		for (i = 0; i < N; i++) {
		
			fprintf (pos, "%Lf \t %Lf \n", rand_num (-L / 2, L / 2), rand_num (-L / 2, L / 2));
		}

	}
	
	else if (d == 3) {
		
		L = pow (N / rho, 1. / 3.);
		
		for (i = 0; i < N; i++) {
		
			fprintf (pos, "%Lf \t %Lf \t %Lf \n", rand_num (-L / 2, L / 2), rand_num (-L / 2, L / 2), rand_num (-L / 2, L / 2));

		}
	}
	
	fclose (pos);

	printf ("L = %0.8Lf \n", L);
}






