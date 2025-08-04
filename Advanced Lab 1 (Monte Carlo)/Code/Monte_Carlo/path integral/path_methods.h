typedef long double ld;

// Function pointers for the quantum systems considered

typedef ld (*V_p) (ld);

typedef ld (*psi_p) (ld);

ld rand_num (long int , long int);

ld *rand_arr (long int, long int, long int);

ld *lin_arr (float, float, int);

void print_arr (ld *, size_t);

void check (ld *, float, float);

void move (ld *, ld *, float, float);

ld E_avg (ld *, size_t, int);

ld path (ld *, ld *, size_t, int);

void update (ld*, ld *, size_t);

void count (ld *, size_t , ld *, size_t, float, float, ld);

void normalize (ld *, size_t, size_t, ld, long int);

ld V_ho (ld);

ld V_sw (ld);

ld psi_ho (ld);

ld psi_sw (ld);

void exact_sol (float, float, size_t);





