typedef long double ld;

// Brownian motion

ld rand_num (int, int);

ld norm_rand (void);

void move (ld *, ld *, size_t, ld *);

void update (ld *, ld *, size_t);

// Forces

ld *F_Hooke (ld *, size_t, ld);

// Memory management

ld ** Nalloc (int, size_t);

void Ndealloc (int, ld **);

// File name management 

char str (int n);

int count_digits (int);

char *int_to_str (int );

char *file_name (int);

