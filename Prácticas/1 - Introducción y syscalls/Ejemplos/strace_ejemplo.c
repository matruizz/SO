#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    if (fork() == 0) {
        printf("Hijo: Hola SO!\n");
    } else {
        printf("Padre: Hola SO!\n");
    }
    exit(EXIT_SUCCESS);
}
