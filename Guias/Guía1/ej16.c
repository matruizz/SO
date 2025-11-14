#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

enum {READ, WRITE};

void procesoHijo1(int pipes[][2]){

    close(pipes[2][READ]);
    close(pipes[2][WRITE]);
    close(pipes[1][READ]);
    close(pipes[0][WRITE]);

    int n = 0;

    while (read(pipes[0][READ], &n, sizeof(n)) != 0)
    {
        if (n < 50)
        {
            n++;
            printf("El hijo 1 envia al hijo 2 el valor %d\n", n);
            write(pipes[1][WRITE], &n, sizeof(n));
        }else{
            close(pipes[0][READ]);
            close(pipes[1][WRITE]);
            exit(EXIT_SUCCESS);
        }
    }
}

void procesoHijo2(int pipes[][2]){

    close(pipes[0][READ]);
    close(pipes[0][WRITE]);
    close(pipes[1][WRITE]);
    close(pipes[2][READ]);

    int n = 0;

    while (read(pipes[1][READ], &n, sizeof(n)) != 0)
    {
        if (n < 50)
        {
            n++;
            printf("El hijo 2 envia al padre el valor %d\n", n);
            write(pipes[2][WRITE], &n, sizeof(n));
        }else{
            close(pipes[1][READ]);
            close(pipes[2][WRITE]);
            exit(EXIT_SUCCESS);
        }
    }
}

void procesoPadre(int pipes[][2]){

    close(pipes[1][READ]);
    close(pipes[1][WRITE]);
    close(pipes[0][READ]);
    close(pipes[2][WRITE]);

    int n = 0;

    printf("El padre envia al hijo 1 el valor %d\n", n);

    write(pipes[0][WRITE], &n, sizeof(n));

    while (read(pipes[2][READ], &n, sizeof(n)) != 0)
    {
        if (n < 50)
        {
            n++;
            printf("El padre envia al hijo 1 el valor %d\n", n);
            write(pipes[0][WRITE], &n, sizeof(n));
        }else{
            close(pipes[0][WRITE]);
            close(pipes[2][READ]);
            exit(EXIT_SUCCESS);
        }
    }
}

int main(){

    int pidPadre = getpid();
    int children[3];
    int pipes[3][2];
    
    for (int i = 0; i < 3; i++)
    {
        pipe(pipes[i]);
    }
    
    for (int i = 0; i < 2; i++)
    {
        children[i] = fork();
    }

    if (children[0] == 0)
    {
        procesoHijo1(pipes);
    }else if(children[1] == 0){
        procesoHijo2(pipes);
    }else{
        procesoPadre(pipes);
    }

    return 0;
}