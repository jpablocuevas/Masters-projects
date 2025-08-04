#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){

    FILE* test_ellipse;

    test_ellipse=fopen("ellipse.txt","w");

    fputs ("x \t y \n", test_ellipse);

    float t=0, a=1, b=2.5, dt=0.03, pi=3.14159265;

    while (t<2*pi){

        fprintf(test_ellipse,"%f \t %f \t \n",a*cos(t),b*sin(t));
        t=t+dt;
    }

    fclose(test_ellipse);

    return 0;
}