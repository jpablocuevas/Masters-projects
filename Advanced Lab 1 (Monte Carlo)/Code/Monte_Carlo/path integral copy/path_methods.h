typedef long double ld;

// Function pointers for the quantum systems considered

typedef ld (*psip) (ld, ld);

ld rand_num (long int , long int);

ld *rand_arr (long int N, long int inf, long int sup);

ld *lin_arr (int, int, int);

void print_arr (ld *, size_t);

void move (ld *, ld *, ld);

ld avg_E (ld *, size_t, int, ld, ld, ld);

ld path (ld *, ld *, size_t, int, ld, ld, ld);

void update (ld*, ld *, size_t);

void count (ld *, size_t , ld *, size_t, long int, long int, ld);

void normalize (ld *, size_t, size_t, ld, long int);

ld V_ho (ld, ld, ld);

ld V_sw (ld);

ld psi_ho (ld, ld, ld);

ld psi_sw (ld, ld);

