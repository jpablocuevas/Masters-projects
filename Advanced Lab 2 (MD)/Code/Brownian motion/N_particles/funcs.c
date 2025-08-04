# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "funcs.h"

// External parameters

extern ld T, kB, D, dt, pi;

// ---------------------------- Brownian dynamics ----------------------------

// Random number generator

ld rand_num (int inf, int sup) {
	
	return   (ld) (rand () % (labs (sup-inf) + 1) + inf) + (ld) rand () / RAND_MAX;
}

// Random number generator with a normal distribution

ld norm_rand (void) {

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

void move (ld *x_new, ld *x_old, size_t x_size, ld *F) {
	
	for (int i = 0; i < x_size; i++) {
	
		*(x_new + i) = *(x_old + i) + D / (kB*T) * dt * ( *(F+i) ) + sqrt (2*D*dt) * norm_rand ();
		
	}
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

// ---------------------------- Memory management ----------------------------

ld ** Nalloc (int N, size_t x_size) {

	ld **x = calloc (N, sizeof (ld *));
	
	for (int i = 0; i < N; i++) {
	
		*(x + i) = calloc (x_size, sizeof (ld));
	}
	
	return x;
}

void Ndealloc (int N, ld **x) {

	for (int i = 0; i < N; i++) {
	
		free (*(x + i));
	}
	
	free (x);
}


// ---------------------------- File naming ----------------------------

char str (int n) {

	char c;

	switch (n) {

	case 0:
	    c = '0';
	    break;

	case 1:
	    c = '1';
	    break;

	case 2:
	    c = '2';
	    break;

	case 3:
	    c = '3';
	    break;

	case 4:
	    c = '4';
	    break;

	case 5:
	    c = '5';
	    break;

	case 6:
	    c = '6';
	    break;

	case 7:
	    c = '7';
	    break;

	case 8:
	    c = '8';
	    break;

	case 9:
	    c = '9';
	    break;

	}
    
    return c;

}

int count_digits (int n) {

	int i = 1;
	
	while (n / (int) pow(10,i) > 1){
	
		i++;
	}
	
	return i;
}



char *int_to_str (int n) {

	int i, j, count, x = 0;
	
	count = count_digits (n); 

	char *num;
	
	num = (char*) malloc (sizeof(char)*(count+1));
	
	j = count-1;
	
	
	for (i = 1; i < count +1; i++){ 
	
		num[j] = str((n % (int) pow (10,i)-x)/ (int) pow (10, i-1)) ;
		
		x = n%(int) pow(10,i);
		
		j--;
		
	}
	
	num[count] = '\0';
	
	return num;
}


char *file_name (int n) {

	char text[] = ".txt";
	char *name, *num;
	
	int i;
	size_t num_size = count_digits (n), text_size;
	
	text_size = sizeof (text) / sizeof (text[0]);
	
	num = (char*) int_to_str(n);

	name = (char*) malloc (sizeof (char) * (num_size+text_size+1));
	
	for (i = 0; i < num_size; i++){
	
		name [i] = num[i];
	}
	
	int j = 0;
	
	for (i = num_size; i <= num_size+text_size; i++){
	
		name [i] = text[j];
		
		j++;
	}
	
	name [num_size+text_size] = '\0';
	
	free (num);
	
	return name;
}
	







