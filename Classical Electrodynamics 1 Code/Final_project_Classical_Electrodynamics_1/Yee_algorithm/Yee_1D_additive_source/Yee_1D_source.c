#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Functions declarations

double * alloc_1D (int size);

void space_file (char*, double*, double*, int);

// Function that dynamically allocates a 1D array

double *alloc_1D (int size){

    double *f;

    f = (double*) calloc (size, sizeof(double));

    return f;
}



int main (void){

    // Physical parameters of the simulation

    double mu_0=1.256*pow(10,-6), epsilon_0=8.854*pow(10,-12); // Electric permititivy and magnetic susceptibility in vaccym
    double Z_0=sqrt(mu_0/epsilon_0); // Impedance in vaccum
    double Sc=1.; // Courant's number

    // Allocation of the arrays for the fields

    int Ez_size = 200, Hy_size = 200;

    double *Ez, *Hy;

    Ez = alloc_1D (Ez_size);
    Hy = alloc_1D (Hy_size);

    int i, t, t_max = 200 ;

    // File allocation

    FILE *space_plot;

    char name[6] = "A.txt", letter [21] = "ABCDEFGHIJKLMNOPQRST";

    // Allocated array for the space index used for the data in the files

    double *pos;

    pos = alloc_1D (Ez_size);

    for (i = 0; i< Ez_size; i++){ // Spatial array for the plots
        pos[i]=(double)i;
    }


    // Update equations. FDTD method


    for ( t = 0; t < t_max; t++){ // Temporal loop

        for (i=0;i<Hy_size-1;i++){ // Hy loop
            Hy[i]=Hy[i]+(Ez[i+1]-Ez[i])/Z_0;
        }

         for (i=1;i<Ez_size;i++){ // Ez loop 
            Ez[i]=Ez[i]+Z_0*(Hy[i]-Hy[i-1]);
        }


        // Adittive source

        Ez[50] = Ez[50] + exp (-(t-30.)*(t-30.)/100.);

        // Producing a file only when t is a multiple of 10 

         if (t % 10 == 0){

            name[0] = letter[(int)t/10];

            //space_file (name, Ez, pos, Ez_size);

            space_plot = fopen (name, "w");

            fputs ("Ez \t x \n", space_plot);

            for (i = 0; i < Ez_size; i++){

                fprintf(space_plot, "%lf \t %lf \n",Ez[i], pos[i]);

            }

            fclose (space_plot);

        }
            
    }

    // Freeing the position array 

    free (pos);

    // Freeing the dynamically allocated arrays for the fields

    free (Ez);
    free (Hy);

    return 0;
}