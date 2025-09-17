#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
    printf("Soy Juan\n");

    pid_t pid = fork();
    if (pid == 0) {
        printf("Soy Julieta\n");

        // Creamos a Jennifer
        pid = fork();
        if (pid == 0) {
            printf("Soy Jennifer\n");
        } else {
            // Acá soy Julieta
            // Esperamos a Jennifer
            wait(NULL);
        }
    } else {
        // Sigo siendo Juan
        // Esperamos a Julieta
        wait(NULL);

        // Creamos a Jorge
        pid = fork();
        if (pid == 0) {
            printf("Soy Jorge\n");
        } else {
            // Acá soy Juan
        }
    }

    // Esperamos a Jorge
    wait(NULL);
}
