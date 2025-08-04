# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <time.h>
# include "methods.h"


ld rand_num (int a, int b, int d) {

	return  (ld) (rand () % abs (b-a) ) + (ld) (rand () % (int) pow (10, d -1)) / pow (10, d-1);
}

ld H (ld x, ld y) {

	if ( pow (x, 2) + pow (y, 2) <= 1.) {
	
		return 1.;
	}
	
	else {
	
		return 0.;
	}
	
	return 0.;
}

ld Q (int N, int a, int b, int d) {

	ld S  = 0.;
	int i;
	
	for (i = 0; i < N; i ++) {
	
		S = S + H (rand_num (a, b, d), rand_num (a, b, d));
	}
	
	return 4 *S/N;
}

