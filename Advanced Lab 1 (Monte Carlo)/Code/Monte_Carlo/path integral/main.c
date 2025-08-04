# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <time.h>
# include "path_methods.h"

// Physical parameters

ld hbar = 1., pi = 3.14159265359;

ld m = 1., omega = 1., a = 2.;

// Acceptance rate

int rate;

// Simulation parameters

ld alpha = 0.1, tau = 0.02; // Maximum displacement and time step

// For HO: alpha = 0.4, tau = 0.35 acc = 40%
// For SW: alpha = 0.1, tau =

char P = 'S'; // 'O' stands for harmonic oscillator, 'S' stands for square well

int main (void) {
	
	long int i, j, N, C = 10000000; // Path size, number of cycles
	
	ld *x_old, *x_new; // Arrays of random points
	
	float inf, sup;
	
	// time_t seed;

	srand ((unsigned) time (NULL));

	// Initial configuration from a text file

	FILE *rand_data = fopen ("rand_data.txt", "r");
	FILE *rand_nums = fopen ("rand_nums.txt", "r");
	
	fscanf (rand_data, "%ld \t %f \t %f", &N, &inf, &sup);
	
	fclose (rand_data);
	
	// Memory allocation 
	
	x_old = (ld *) malloc (sizeof (ld)*N);
	x_new = (ld *) calloc (N, sizeof (ld));
	
	for (i = 0; i < N; i++) {
	
		fscanf (rand_nums, "%Lf \n", &x_old[i]);
	}
	
	fclose (rand_nums);	
	
	// Equating the new and old configurations ONLY for the first step;
	
	update (x_new, x_old, N);
	
	//print_arr (x_new, N);

	// Statistics 
	
	FILE *mc = fopen ("monte_carlo.txt", "w");
	
	int M = 40; // Number of boxes
	
	ld *psi_count; // Stores the frequency of the points in each box
	
	psi_count = (ld *) calloc (M, sizeof (ld));
	
	ld step = (ld) fabsf (sup - inf) / (ld) M;
	
	// Main loop
	
	rate = 0;
	
	ld E = 0.;
	
	for (i = 0; i < C; i++) {
	
		j =  rand () % N;
		
		move (&x_old[j], &x_new[j], inf, sup);
		
		E = E + path (x_old, x_new, N, j);
	
		count (x_new, N, psi_count, M, inf, sup, step);
	}
	
	// Average energy 
	
	printf ("E_avg = %.10Lf \n", E / (N*C));
	
	printf ("Acceptance rate = %f \n",(rate / (float) C) * 100);
	
	// Normalizing the frequency array 
	
	normalize (psi_count, M, N, step, C);

	// Writing the frequency array to a file
	
	for (i = 0; i < M; i ++) {
		
		// Middle point of the i-th box to plot the i-th element of psi_count
		
		fprintf (mc, "%.10Lf \t %.10Lf \n", inf + i*step + step / 2. , psi_count[i]);
	}
	
	fclose (mc);
	
	free (psi_count);
	

	free (x_old);
	free (x_new);
	
	// Analytical solution
	
	int n = 200; // Number of points for the analytical solution
	
	exact_sol (inf, sup, n);

	return 0;
}









