#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


void procesoBart(){
    printf("Soy Bart\n");
    exit(EXIT_SUCCESS);
};

void procesoLisa(){
    printf("Soy Lisa\n");
    exit(EXIT_SUCCESS);
};

void procesoMaggie(){
    printf("Soy Maggie\n");
    exit(EXIT_SUCCESS);
};

void procesoHomero(){
    printf("Soy Homero\n");
    int bart = fork();
    if (bart == 0)
    {
        procesoBart();
    }
//    wait(NULL);
    int lisa = fork();
    if (lisa == 0)
    {
        procesoLisa();
    }
//    wait(NULL);
    int maggie = fork();
    if (maggie == 0)
    {
        procesoMaggie();
    }
//    wait(NULL);
    exit(EXIT_SUCCESS);

}



int main(){
    printf("SOy Abraham.\n");
    int homero = fork();
    if (homero == 0)
    {
        procesoHomero();
    }
//    wait(NULL);
    return 0;
}