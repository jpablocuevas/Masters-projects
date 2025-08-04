# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <time.h>
# include "path_methods.h"

ld pi = 3.14159265359;

int main (void) {
	
	long int i, j, N, C = 10000000; // Path size, number of cycles
	
	ld alpha = 1., tau = 1.5; // Maximum displacement and time step
	
	ld *x_old, *x_new; // Arrays of random points
	
	float inf, sup;
	
	time_t seed = 58392;

	srand ((unsigned) time (&seed));

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
	
	print_arr (x_new, N);

	// Statistics 
	
	FILE *mc = fopen ("monte_carlo.txt", "w");
	
	int M = 40; // Number of boxes
	
	ld *psi_count; // Stores the frequency of the points in each box
	
	psi_count = (ld *) calloc (M, sizeof (int));
	
	ld step = (ld) fabsf (sup - inf) / (ld) M;
	
	// Main loop
	
	ld m = 1., omega = 1., E = 0.;
	
	for (i = 0; i < C; i++) {
	
		j =  rand () % N;
		
		move (&x_old[j], &x_new[j], alpha);
		
		E = E + path (x_old, x_new, N, j, tau, m, omega);
		
		//if (C % i == 50) {
			
			count (x_new, N, psi_count, M, inf, sup, step);
		//}
	}
	
	// Average energy 
	
	printf ("E_avg = %.10Lf \n", E / (ld) C);
	
	// Normalizing the frequency array 
	
	normalize (psi_count, M, step, C);

	// Writing the frequency array to a file
	
	for (i = 0; i < M; i ++) {
		
		// Middle point of the i-th box to plot the i-th element of psi_count
		
		fprintf (mc, "%.10Lf \t %.10Lf \n", inf + i*step + step / 2., psi_count[i]);
	}
	
	fclose (mc);
	
	free (psi_count);
	

	free (x_old);
	free (x_new);
	
	// Analytical solution
	
	FILE *exact;
	
	exact = fopen ("exact.txt", "w");
	
	fputs ("x \t psi_0 (x) \n", exact);
	
	ld *x_ex, *psi;
	
	int n_pts = 200;
	
	x_ex = (ld *) lin_arr (inf, sup, n_pts);
	
	for (i = 0; i < n_pts; i++ ) {
		
		fprintf (exact, "%Lf \t %Lf \n", x_ex[i], (ld) pow (psi_ho (x_ex[i], m, omega), 2) );
	}
	
	fclose (exact);
	
	free (x_ex);
	free (psi);

	return 0;
}









