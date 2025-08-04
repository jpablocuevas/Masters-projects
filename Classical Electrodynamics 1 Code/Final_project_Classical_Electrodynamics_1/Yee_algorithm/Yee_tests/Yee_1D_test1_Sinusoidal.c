#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int main (void){    

    // Physical parameters of the simulation

    double mu_0=1.256*pow(10,-6), epsilon_0=8.854*pow(10,-12); // Electric permititivy and magnetic susceptibility in vaccym
    double Z_0=sqrt(mu_0/epsilon_0); // Impedance in vaccum
    double Sc=1.; // Courant's number 

    // Update equations for Hy and Ez. Time is an implicit variable an has its own loop.
    int t, i, j, t_max=250, Hy_max=200, Ez_max=200;

    // Allocation of arrays

    double *Ez, *Hy;

    Ez=(double*)calloc(Ez_max, sizeof(double));
    Hy=(double*)calloc(Hy_max, sizeof(double));    

    // File managing
    
    FILE* time_plot;

    time_plot = fopen ("Ez_plot_sinusoidal.txt", "w");

    fputs ("Ez \t t \n", time_plot);


    // Update equations for the fields.

    for (t=0;t<t_max; t++){ // Time loop 


        for (i=0;i<Hy_max-1;i++){ // Hy loop
            Hy[i]=Hy[i]+(Ez[i+1]-Ez[i])/Z_0;
        }

         for (i=1;i<Ez_max;i++){ // Ez loop 
            Ez[i]=Ez[i]+Z_0*(Hy[i]-Hy[i-1]);
        }

         Ez[0] = sin((double)t / sqrt(mu_0*epsilon_0));

        fprintf(time_plot, "%lf \t %lf \n",Ez[50], (double)t );
        
    }

        fclose(time_plot);

        free (Ez);
        free (Hy);

        return 0;
    }