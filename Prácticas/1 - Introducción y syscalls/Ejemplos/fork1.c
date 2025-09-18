#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void subrutina_proceso_hijo() {
    for (int i = 0; i < 3; ++i) {
        sleep(1);
        write(1,"Soy el proceso hijo\n",20);
    }
    exit(EXIT_SUCCESS);
}

void subrutina_proceso_padre(pid_t pid) {
    printf("PID hijo: %d\n", pid);
    for (int i = 0; i < 3; ++i) {
        sleep(1);
        write(1,"Soy el proceso padre\n",21);
    }
}

int main(int argc, char const *argv[]) {
    pid_t pid = fork();

    // si es -1, hubo un error
    if (pid == -1) exit(EXIT_FAILURE);

    if (pid == 0) {
        // si no hay error, pid vale 0 para el hijo
        subrutina_proceso_hijo();
    } else {
        // y el valor del process id del hijo para el padre
        subrutina_proceso_padre(pid);
    }

    // cada uno finaliza su proceso
    exit(EXIT_SUCCESS);
}
