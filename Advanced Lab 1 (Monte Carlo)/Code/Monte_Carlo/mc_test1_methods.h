typedef long double ld;

ld rand_num (int, int);

ld dis2 (ld *, size_t);

ld *next_pos (ld *, size_t, ld, int, int);

ld check (ld, ld);

void update (ld *, ld *, size_t);

ld **alloc_2d (int, int);

void dealloc_2d (ld **, int);
