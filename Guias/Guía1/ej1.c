#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

enum {READ, WRITE};

void procesoHijo(pid_t pidPadre, int pipe[2]){
    int msg = 0;

    while (1)
    {
        msg = breceive(pidPadre);
        printf("El padre envia al hijo el valor %d", msg + 1);
        bsend(pidPadre, msg + 1);
    }
}

void procesoPadre(pid_t pidHijo, int pipe[2]){

    int msg = 0;
    
    sleep(1);

    printf("El padre envia al hijo el valor %d", msg + 1);
    //bsend(pidHijo, msg);
    write(pipe[WRITE], &msg, sizeof(msg));

    sleep(1);
    while (1)
    {
        //msg = breceive(pidHijo);
        read(pipe[READ], &msg, sizeof(msg));

        printf("El padre envia al hijo el valor %d", msg + 1);
        //bsend(pidHijo, msg + 1);
        write(pipe[WRITE], &msg, sizeof(msg));
    }
    
}

int main(){

    int pidPadre = getpid();
    int pipeFD[2];
    pipe(pipeFD);

    int pidHijo = fork();

    if (pidHijo == 0)
    {
        procesoHijo(pidPadre, pipeFD);
    }else{
        procesoPadre(pidHijo, pipeFD);
    }

    return 0;
}