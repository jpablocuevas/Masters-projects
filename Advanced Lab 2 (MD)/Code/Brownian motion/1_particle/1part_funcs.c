# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "1part_funcs.h"

// External parameters

extern ld T, kB, D, dt, pi;

// ---------------------------- Brownian dynamics ----------------------------

// Random number generator

ld rand_num (int inf, int sup) {
	
	return   (ld) (rand () % (labs (sup-inf) + 1) + inf) + (ld) rand () / RAND_MAX;
}

// Random number generator with a normal distribution

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

// Ermak-McCammon EOM 

ld *move (ld *x_old, size_t x_size, ld *F) {
		
	ld *x_new = malloc (sizeof (ld) * x_size);
	
	for (int i = 0; i < x_size; i++) {
	
		*(x_new + i) = *(x_old + i) + D / (kB*T) * dt * ( *(F+i) ) + sqrt (2*D*dt) * gauss_rand ();
		
	}
	
	return x_new;
}

// Updates new and old coordinates

void update (ld *x_new, ld *x_old, size_t x_size) {

	for (int i = 0; i < x_size; i++) {
	
		* (x_old + i) = *(x_new + i);
	}
}
// ---------------------------- External force functions ----------------------------


// Hook's law

ld *F_Hooke (ld *x, size_t x_size, ld k) {

	ld *F = calloc (x_size, sizeof (ld));

	for (int i = 0; i < x_size; i++) {
		
		printf ("a");
	}
	
	return F;
}









