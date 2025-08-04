#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main (void){

    char X[20]="ABCD";
    char *file_name;


    FILE* letter;

    int i;

    for (i=0;i<strlen(X);i++){
        
        file_name=(char*)malloc(sizeof(char)*6);

        file_name[0]=X[i];
        letter=fopen(strcat(file_name,".txt\0"),"w");

        fprintf(letter,"%c",X[i]);
        fclose(letter);

        file_name=NULL;

    }

    return 0;
}