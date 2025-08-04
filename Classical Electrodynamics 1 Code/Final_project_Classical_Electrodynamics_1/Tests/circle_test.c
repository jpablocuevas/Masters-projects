#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){

    FILE* test_circle;

    test_circle=fopen("circle.txt","w");

    fputs ("x \t y \n", test_circle);

    float theta=0, r=2, dtheta=0.03, pi=3.14158265;

    while (theta<2*pi){

        fprintf(test_circle,"%f \t %f \t \n",r*cos(theta),r*sin(theta));
        theta=theta+dtheta;
    }

    fclose(test_circle);

    return 0;
}