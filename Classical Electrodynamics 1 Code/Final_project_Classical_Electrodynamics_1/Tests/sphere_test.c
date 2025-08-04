#include <stdio.h>
#include <math.h>


int main (void){

    FILE* sphere;

    sphere = fopen("sphere.txt", "w");

    float r=1, theta = 0, phi = 0, dphi=0.1, dtheta = 0.1;
    float pi=3.14159265;

    fputs("x \t y \t z \t \n", sphere);

    while (phi<2*pi){
        while (theta<pi){
            fprintf(sphere,"%f \t %f \t %f \t \n",r*sin(theta)*cos(phi),r*sin(theta)*sin(phi), r*cos(theta));
            theta=theta+dtheta;
        }
        theta =0;
        phi=phi+dphi;
    }

    fclose(sphere);

    return 0;
}