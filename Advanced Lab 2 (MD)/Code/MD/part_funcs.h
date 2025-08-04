
typedef long double ld;

// Random numbers

ld rand_num (int, int);

ld gauss_rand (void);



// Gives the distance between two points

ld dis (ld *, ld *, size_t);


// ------------------------- Grid initialization -------------------------

// Regular lattice initialization 

void grid_init (int, ld, ld, int);

// Random grid initialization 

void rand_init(int, ld, ld, int);

