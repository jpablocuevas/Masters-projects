#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function declarations

double ** alloc_2D (int, int);

double initial_field (double, double, double, double, char);

void free_alloc_2D (double**, int);

double **coeff (char*, int, int, double);

// Function that returns a dynamically allocated double pointer (matrix array)

double **alloc_2D (int x_size, int y_size){

    double **f;

    f= (double**) calloc (x_size, sizeof(double*));

    int i;

    for (i=0;i<x_size;i++){
        f[i]= (double*) calloc (y_size, sizeof(double)); 
    }

    return f;

}

// Function that returns the initial conditions for the fields

double initial_field (double x, double y , double t, double alpha, char F){

    double pi = 3.14159265, c = 3.8*pow(10,8), Z = 376.7;

    if (F=='E'){

        return sin((x-50*alpha+c*t)*pi/(8*alpha));
    }

    else if (F=='H'){
        return (1/Z)*sin((x-5*alpha+c*t)*pi/(8*alpha));

    }
    return 0;

}

// Function that frees the 2D dynamically allocated arrays

void free_alloc_2D (double **f, int size){

    int i;

    for (i=0; i<size; i++){
        free (f[i]);
    }

    free (f);
}

// Function that returns the appropiate coefficients for the update equations of the fields

double **coeff (char* name, int x_size, int y_size, double Sc){

    // Allocating memory for the arrays

    double **C;

    C = (double**) calloc (x_size, sizeof(double*));

    int i, j;

    for (i=0; i<x_size; i++){
        C[i] = (double*) calloc (y_size, sizeof (double));
    }

    // Filling up the arrays


}

int main (void){


    // Grid size

    int i, j, t, t_max=200;
    int Ez_size_i=250, Ez_size_j=250, Hx_size_i=250, Hx_size_j=250, Hy_size_i=250, Hy_size_j=250;

    // Memory allocation for the fields

    double **Ez, **Hy, **Hx;

    Ez = alloc_2D (Ez_size_i, Ez_size_j);
    Hx = alloc_2D (Hx_size_i, Hx_size_j);
    Hy = alloc_2D (Hy_size_i, Hy_size_j);

    // Memory allocation for the coefficients

    double **CHxH, **CHxE, **CHyH, **CHyE, **CEzE, **CEzH;

    CEzE = alloc_2D (Ez_size_i, Ez_size_j);
    CEzH = alloc_2D (Ez_size_i, Ez_size_j);

    CHxH = alloc_2D (Hx_size_i, Hx_size_j-1);
    CHxE = alloc_2D (Hx_size_i, Hx_size_j-1);

    CHyH = alloc_2D (Hy_size_i-1, Hy_size_j);
    CHyE = alloc_2D (Hy_size_i-1, Hy_size_j);

    
    // Physical parameters for the simulation

    double Z_0 = 376.7, Sc = 1./sqrt(2), alpha=2.;


    // Update equations

    for (t=0; t<t_max; t++){ // Time loop 

        // Hx field loop 
        for (i=0; i<Hx_size_i; i++){
            for (j=0; j<Hx_size_j-1; j++){

                Hx[i][j] = CHxH[i][j]*Hx[i][j]-CHxE[i][j]*(Ez[i][j+1]-Ez[i][j]);
            
            }
        }

        // Hy field loop

        for (i=0; i<Hy_size_i-1; i++){
            for (j=0; j<Hy_size_i; j++){

                Hy[i][j]= CHyH[i][j]*Hy[i][j]+CHyE[i][j]*(Ez[i+1][j]-Ez[i][j]);
            
            }
        }

        // Ez field loop
        for (i=1; i<Ez_size_i; i++){
            for (j=1; j<Ez_size_i; j++){

                Ez[i][j]=CEzE[i][j]*Ez[i][j]+CEzH[i][j]*(Hy[i][j]-Hy[i-1][j]-(Hx[i][j]-Hx[i][j-1])); 

                //printf("%g \n",Ez[i][j]);           
            }
        }

        // Initial conditions

        Ez[0][0]=initial_field (0, 0, t, alpha, 'E');
        Hy[0][0]=initial_field (0, 0, t, alpha, 'H');

        
    }

    // Freeing the allocated memory for the fields

    free_alloc_2D (Ez, Ez_size_i);

    free_alloc_2D (Hx, Hx_size_i);

    free_alloc_2D (Hy, Hy_size_i);

    // Freeing the allocated memory for the coefficients

    free_alloc_2D (CEzE, Ez_size_i);
    free_alloc_2D (CEzH, Ez_size_i);


    free_alloc_2D (CHxH, Hx_size_i);
    free_alloc_2D (CHxE, Hx_size_i);


    free_alloc_2D (CHyH, Hy_size_i-1);
    free_alloc_2D (CHyE, Hy_size_i-1);


    return 0;
}

