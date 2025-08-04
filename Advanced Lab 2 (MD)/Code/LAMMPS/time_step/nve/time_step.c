# include <stdio.h>
# include <stdlib.h>
# include <math.h>

// Computes the mean and std of an array of data

float *stats (float *arr, size_t arr_size) {

	float *data = malloc (2 * sizeof (float));
	
	float mu = 0., sigma = 0.;
	
	int i;
	
	// Mean 
	
	for (i = 0; i < arr_size; i ++) {
	
		mu = mu + arr[i];
	}
	
	mu = mu / arr_size;
	
	// Standard deviation
	
	for (i = 0; i < arr_size; i ++) {
	
		sigma = sigma + pow (arr[i] - mu, 2);
	}
	
	sigma = sqrt (sigma);
	
	data[0] = mu;
	data[1] = sigma;
	
	return data;
}


int main (void) {

	FILE* data;
	
	data = fopen ("data.txt", "r");
	
	int T_size = 100;
	
	float *T, *dat;
	
	T = malloc (T_size * sizeof (float));
	
	int i;
	
	for (i = 0; i < T_size; i ++) {
		
		fscanf (data, "%f \n", &T[i]);
	}
	
	fclose (data);
	
	dat = stats (T, T_size);
	
	printf (" mu = %f, sigma = %f \n", dat[0], dat[1]);
	
	free (dat);
	
	free (T);

	return 0;
}




