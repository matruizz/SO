#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    printf("Hola SO!\n");
    pid_t pidOrZero = fork();
    printf("El valor retornado de fork es: %d\n", pidOrZero);
    exit(EXIT_SUCCESS); //cada uno finaliza su proceso
}
