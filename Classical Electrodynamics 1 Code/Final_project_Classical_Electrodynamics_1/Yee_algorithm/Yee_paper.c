#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function declarations

double** field (int, int);

double initial_field (double, double, double, double, char);

void free_field (double**, int);

// void coef (char*, v );

// Function that returns a dynamically allocated double pointer (matrix array)

double **field (int x_size, int y_size){

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
        return (1/Z)*sin((x-50*alpha+c*t)*pi/(8*alpha));

    }
    return 0;

}

// Function that frees the 2D dynamically allocated arrays

void free_field (double **f, int x_size){

    int i;

    for (i=0; i<x_size; i++){
        free (f[i]);
    }

    free (f);
}

int main (void){


    // Grid size

    int i, j, t, t_max=200;
    int Ez_size_i=250, Ez_size_j=250, Hx_size_i=250, Hx_size_j=250, Hy_size_i=250, Hy_size_j=250;

    // Memory allocation for the fields

    double **Ez, **Hy, **Hx;

    Ez = field (Ez_size_i, Ez_size_j);
    Hx = field (Hx_size_i, Hx_size_j);
    Hy = field (Hy_size_i, Hy_size_j);

    
    // Physical parameters for the simulation

    double alpha = 1.; // Square's side length

    double Z = 376.7, dx = alpha/8, dy = alpha/8, dt= alpha/16;


    // Update equations

    for (t=0; t<t_max; t++){ // Time loop 

        // Hx field loop 
        for (i=0; i<Hx_size_i; i++){
            for (j=0; j<Hx_size_j-1; j++){

                Hx[i][j]= Hx[i][j]-(1/Z)*(dt/dy)*(Ez[i][j+1]-Ez[i][j]);
            
            }
        }

        // Hy field loop

        for (i=0; i<Hy_size_i-1; i++){
            for (j=0; j<Hy_size_i; j++){

                Hy[i][j]= Hy[i][j]+(1/Z)*(dt/dx)*(Ez[i+1][j]-Ez[i][j]);
            
            }
        }

        // Ez field loop
        for (i=1; i<Ez_size_i; i++){
            for (j=1; j<Ez_size_i; j++){

                Ez[i][j]=Ez[i][j]+Z*(dt/dx)*(Hy[i][j]-Hy[i-1][j])-Z*(dt/dy)*(Hx[i][j]-Hx[i][j-1]); 

            }
        }

        // Initial conditions

        Ez[0][0]=initial_field (0, 0, t, alpha, 'E');
        Hy[0][0]=initial_field (0, 0, t, alpha, 'H');

        printf("%e \n",Ez[0][0]);
        
    }

    // Freeing the allocated memory

    free_field (Ez, Ez_size_i);

    free_field (Hx, Hx_size_i);

    free_field (Hy, Hy_size_i);

    return 0;
}

