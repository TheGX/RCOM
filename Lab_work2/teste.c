#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){

    
    char teste [] = "CPAN/CPANaaa/ola/file.c";
    int done=0;
    char *filename;
    int l=0;
    char *user_aux;

    //while(!done){

       /* if(strstr(user_aux, "/")==NULL){

            printf("acabei\n");
            done=1;
        }*/

    user_aux=strtok(teste, "/");
    printf("aux0: %s \n", user_aux);


    while((user_aux=strtok(0, "/"))!= NULL){
       
       // memset(filename,0, strlen(filename));
        filename=user_aux;
    }

    printf("filename: %s \n", filename);
}