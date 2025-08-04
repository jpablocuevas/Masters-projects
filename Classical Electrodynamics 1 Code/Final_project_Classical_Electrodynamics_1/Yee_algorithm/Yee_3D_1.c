#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function definitions

double ***field (int, int, int);

void free_field (double ***, int, int, int);

double ***coef (int, int, int);



// Generates a 3D dynamically allocated array using calloc

double ***field (int x_size, int y_size, int z_size){

    int i, j, k;

    double *** f;

    f = (double***) calloc (x_size, sizeof(double**));

    for (i=0; i<x_size; i++){
        f[i] = (double**) calloc (y_size, sizeof(double*));

        for (j=0; j< y_size; j++){
            f[i][j]= (double*) calloc (z_size, sizeof(double));
        }
    }
        return f;
    }

// Frees a 3D dynamically allocated array

void free_field (double ***f, int x_size, int y_size, int z_size){

    int j, k;

    for (j = 0; j < y_size; j++){

        for (k = 0; k < z_size; k++){

            free (f[j][k]);
        }

        free (f[j]);
    }

    free (f);
}



int main (void){

    int i,j ,k;

    i=j=k=0;


    return 0;
}