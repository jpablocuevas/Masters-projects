# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <time.h>
# include "methods.h"


int main (void) {

	time_t seed;

	srand ((unsigned) time (&seed));

	int N = 1000000, a = 0, b = 1, d = 8;
	
	printf (" pi = %0.8Lf \n", Q (N, a, b, d));

	return 0;
}
