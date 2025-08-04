# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "path_methods.h"

extern ld pi;

// Pseudo-random number generator 
 
ld rand_num (long int inf, long int sup) {

	ld n;
	
	n = (ld) (rand () % (labs (sup-inf) +1) + inf);
	
	if ( n == inf || n == sup) {
	
		if ((ld) labs (sup - inf) == 1.) {
	
			 return ((ld) rand ()) / pow (10,9);
		}
		
		else {
			
			return n;
		}
	}
	
	else {
		
		return n + ((ld) rand ()) / pow (10,10);
	}
	
	return n;
}

// Generates an array of N random points in the interval [inf, sup]

ld *rand_arr (long int N, long int inf, long int sup) {
	
	ld *arr;
	
	arr = (ld*) malloc (sizeof (ld) * N);
	
	arr[0] = rand_num (inf, sup);
	
	for (int i = 1; i < N; i++) {
	
		arr[i] = rand_num (inf, sup);
	}
	
	return arr;
}

// Returns an array of N points of the interval [a,b] 

ld *lin_arr (int a, int b, int N) {

	ld *arr;
	
	arr = (ld *) malloc (sizeof (ld)*N);
	
	ld step = (ld) abs(b-a) / N;
	
	for (int i = 0; i < N; i++) {
	
		arr[i] = a + i*step;
	}
	
	return arr;
}

// Prints an array 

void print_arr (ld *arr, size_t arr_size) {

	printf ("{");
	
	for (int i = 0; i < arr_size - 1; i++) {
	
		printf ("%Lf, \n", arr[i]);
	
	}
	
	printf ("%Lf}", arr[arr_size -1]);
}

// Function that updates the j-th random number x_j given a maximum displacement alpha

void move (ld *x_j_old, ld *x_j_new, ld alpha ) {
	
	*x_j_new = *x_j_old + alpha * (2* (ld) (rand () % (long int) (pow (10,8)) ) / pow(10,8) -1.);
}

// Calculates the average energy of the system 

ld avg_E (ld *x, size_t N, int j, ld tau, ld m, ld omega) {
	
	ld E = 0.;
	
	/*for (int i = 0; i < N; i++ ) {
	
		if (i == j) {
		
			E = E + m / 2. * (ld) pow ((x[0] - x[j]) / tau, 2.) + V_ho (x[0], m, omega);
		}
		
		E = E + m / 2. * (ld) pow ((x[i+1] - x[i]) / tau, 2.) + V_ho (x[i+1], m, omega);
	}*/
	
	for (int i = 0; i < N-1; i++ ) {
		
		E = E + m / 2. * (ld) pow ((x[i+1] - x[i]) / tau, 2.) + V_ho (x[i+1], m, omega);
	}
		
	return E + m / 2. * (ld) pow ((x[j] - x[N-1]) / tau, 2.) + V_ho (x[j], m, omega);
	
	/*for (int i = 0; i < N-1; i++ ) {
	
		E = E + m / 2. * (ld) pow ((x[i+1] - x[i]) / tau, 2.) + V_ho (x[i+1], m, omega);
	}
	
	return E  + m / 2. * (ld) pow ((x[0] - x[N-1]) / tau, 2.) + V_ho (x[0], m, omega);*/
}

// Criteria of rejection for the new paths

ld path (ld *x_old, ld *x_new, size_t N, int j, ld tau, ld m, ld omega) {
	
	ld hbar = 1.;

	ld dE = avg_E (x_new, N, j, tau, m, omega) - avg_E (x_old, N, j, tau, m, omega);
		
	if (dE < 0) {
		
		update (x_old, x_new, N);
	}
	
	else {
	
		if ( (ld) (rand () % (long int) (pow (10,8)) ) / pow(10,8) < exp (- dE * tau / hbar) ) {
			
			update (x_old, x_new, N);
		}
		
		else {
			
			update (x_new, x_old, N);
		}
	}
	
	return avg_E (x_new, N, j, tau, m, omega);
}

// Updates paths

void update (ld* x_old, ld *x_new, size_t N) {

	int i;
	
	for (i = 0; i < N; i++) {
	
		x_old[i] = (ld) x_new[i];
	}
}

// Statistics for the frequency of points in the boxes

// Computes the frequency of the points in the x array in each of the boxes of the interval

void count (ld *x, size_t N, ld *psi_count, size_t M, long int inf, long int sup, ld step) {
	
	for (int i = 0; i < N; i++) {
		
		int j = 0;
		
		while (inf + j*step < sup) {
		
			if (inf + j*step <= x[i] && x[i] <= inf + (j+1)*step) {
			
				psi_count[j] = psi_count[j] + 1;
				
				break;
			}
			
			j++;
		}
	}
}

// Normalizes the psi_count array to generate the histogram

void normalize (ld *psi_count, size_t M, size_t N, ld step, long int C) {

	for (int i = 0; i < M; i++) {
	
		psi_count[i] = psi_count[i] / (C * N*step); // Normalization factor
	}
}

// Potentials

ld V_ho (ld x, ld m, ld omega) {

	return m / 2. * (ld) pow (omega*x, 2);
}

ld V_sw (ld a) {
	
	return 0;
}

// Function that returns the base-state eigenfunction for the harmonic oscillator

ld psi_ho (ld x, ld m, ld omega) {

	//ld hbar = 1.05457182 * (ld) pow (10,- 34);  // m^2 kg / s
	ld hbar = 1.;
	
	return pow(m*omega / (pi*hbar), 1./4.) * exp (-m*omega / (2*hbar) * pow (x, 2.));
}

// Function that returns the base-state eigenfunction for the square well potential

ld psi_sw (ld x, ld a) {

	return sqrt (2./a) * cos (pi*x / a);
}



