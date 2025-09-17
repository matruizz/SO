#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// Este es un ejemplo que no soluciona el problema.
// Usar sleep() para "sincronizar" los procesos está mal.

int main(int argc, char const *argv[]) {
    printf("Soy Juan\n");

    pid_t pid = fork();
    if (pid == 0) {
        printf("Soy Julieta\n");
        sleep(1);

        // Creamos a Jennifer
        pid = fork();
        if (pid == 0) {
            printf("Soy Jennifer\n");
        } else {
            // Acá soy Julieta
        }
    } else {
        // Sigo siendo Juan
        // Esperamos a Julieta
        sleep(1);

        // Creamos a Jorge
        pid = fork();
        if (pid == 0) {
            printf("Soy Jorge\n");
        } else {
            // Acá soy Juan
        }
    }
}
