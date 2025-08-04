# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "path_methods.h"

// External parameters defined in the main file

extern ld hbar, pi, m, omega, a;

extern ld alpha, tau;

extern char P;

extern int rate;

// --------------------- Code functions --------------------- 

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

ld *lin_arr (float a, float b, int N) {

	ld *arr;
	
	arr = (ld *) malloc (sizeof (ld)*N);
	
	ld step = (ld) fabsf(b-a) / (float)N;
	
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

// --------------------- Monte Carlo methods --------------------- 

// Periodic boundary condition for the square well potential

void check (ld *x_j_new, float inf, float sup) {

	if ( *x_j_new < inf ) {
	
		*x_j_new = sup - (ld) fabsl (*x_j_new - inf);
	}
	
	else if (*x_j_new > sup) {
	
		*x_j_new = inf + (ld) fabsl (*x_j_new - sup);
	}
}

// Function that updates the j-th random number x_j given a maximum displacement alpha

void move (ld *x_j_old, ld *x_j_new, float inf, float sup) {
	
	ld eta = (ld) rand () / RAND_MAX;
	
	*x_j_new = *x_j_old + alpha * (2*eta -1.);
	
	
	check (x_j_new, inf, sup); // Receives a pointer variable. Checks for boundary conditions
}

// Calculates the average energy of the system 

ld E_avg (ld *x, size_t N, int j) {
	
	ld E = 0.;
	
	V_p V; // Function pointer to the desired potential for the two systems considered
	
	if (P == 'O') {
		
		V = &V_ho;
	}
	
	else if (P == 'S') {
	
		V = &V_sw;
	}
	
	else {
		
		printf ("Potential not found !");
		
		exit (1);
	}
	
	for (int i = 0; i < N-1; i++ ) {
		
		E = E + m / 2. * (ld) pow ((x[i+1] - x[i]) / tau, 2.) + (*V) (x[i+1]);
	}
		
	return E + m / 2. * (ld) pow ((x[0] - x[N-1]) / tau, 2.) + (*V) (x[0]);
	
}

// Criteria of rejection for the new paths

ld path (ld *x_old, ld *x_new, size_t N, int j) {

	ld dE = E_avg (x_new, N, j) - E_avg (x_old, N, j);
	
	ld eta = (ld) rand () / RAND_MAX;
		
	if (dE < 0) {
		
		update (x_old, x_new, N);
		
		rate ++;
	}
	
	else {
	
		if ( eta < exp (- dE * tau / hbar) ) {
			
			update (x_old, x_new, N);
		}
		
		else {
			
			update (x_new, x_old, N);
		}
	}
	
	return dE;
}

// Updates paths

void update (ld* x_old, ld *x_new, size_t N) {

	int i;
	
	for (i = 0; i < N; i++) {
	
		x_old[i] = (ld) x_new[i];
	}
}

// --------------------- Statistics --------------------- 

// Computes the frequency of the points in the x array in each of the boxes of the interval

void count (ld *x, size_t N, ld *psi_count, size_t M, float inf, float sup, ld step) {
	
	for (int i = 0; i < N; i++) {
		
		int j = 0;
		
		while (inf + j*step < sup) {
		
			if (inf + j*step <= x[i] && x[i] <= inf + (j+1)*step) {
			
				psi_count[j] = psi_count[j] + 1.;
				
				break;
			}
			
			j++;
		}
	}
}

// Normalizes the psi_count array to generate the histogram

void normalize (ld *psi_count, size_t M, size_t N, ld step, long int C) {

	for (int i = 0; i < M; i++) {
	
		psi_count[i] = psi_count[i] / (C * N * step); // Normalization factor
	}
}

// --------------------- Potentials --------------------- 

ld V_ho (ld x) {

	return m / 2. * (ld) pow (omega*x, 2);
}

ld V_sw (ld x) {
	
	return 0.;
}

// Function that returns the base-state eigenfunction for the harmonic oscillator

ld psi_ho (ld x) {

	//ld hbar = 1.05457182 * (ld) pow (10,- 34);  // m^2 kg / s
	
	return pow(m*omega / (pi*hbar), 1./4.) * exp (-m*omega / (2*hbar) * pow (x, 2.));
}

// Function that returns the base-state eigenfunction for the square well potential

ld psi_sw (ld x) {

	return sqrt (2./a) * cos (pi*x / a);
}

// Exact solution

void exact_sol (float inf, float sup, size_t n) {

	FILE *exact;
	
	exact = fopen ("exact.txt", "w");
	
	fputs ("x \t psi_0 (x) \n", exact);
	
	ld *x_ex;
	
	x_ex = (ld *) lin_arr (inf, sup, n);
	
	psi_p psi; // Function pointer to the desired psi
	
	if (P == 'O') {
		
		psi = &psi_ho;
	}
	
	else if (P == 'S') {
	
		psi = &psi_sw;
	}
	
	else {
		
		printf ("Potential not found !");
		
		exit (1);
	}
	
	for (int i = 0; i < n; i++ ) {
		
		fprintf (exact, "%Lf \t %Lf \n", x_ex[i], (ld) pow ((*psi) (x_ex[i]), 2) );
		
	}
	
	fclose (exact);
	
	free (x_ex);
}


