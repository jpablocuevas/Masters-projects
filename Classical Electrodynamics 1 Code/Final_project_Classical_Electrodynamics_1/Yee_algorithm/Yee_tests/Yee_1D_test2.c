#include <stdio.h>
#include <stdlib.h>
#include <math.h>



double * H (int);



double* H (int n){

    double* pH=NULL;

    pH=(double*)calloc(n,sizeof(double));

    int i;

    for (i=0;i<n;i++){
        pH[i]=cos(i);
    }

    return pH;

}

int main (void){

    int size=10,i;
    double* pH;

    pH=H(size);

    for (i=0;i<size;i++){
        printf(" H[%d]= %f \n",i,pH[i]);
    }

    printf("%d",sizeof(pH));
    free(pH);

    return 0;
}