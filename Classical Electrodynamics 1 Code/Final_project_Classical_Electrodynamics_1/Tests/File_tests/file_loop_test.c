#include <stdio.h>
#include <stdlib.h>



int main (void){


    FILE* file;

    int i;

    for (i = 0; i<5; i++){
        file = fopen ("file.txt", "w");

        fprintf(file, "%f", (float)i);

        fclose(file);
    }

    return 0;
}