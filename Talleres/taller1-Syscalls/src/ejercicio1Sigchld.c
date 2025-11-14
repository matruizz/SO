#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int n;
int numero_maldito;
 


int pidHijos[10];

int generate_random_number(){
    //Funcion para cargar nueva semilla para el numero aleatorio
    srand(time(NULL) ^ getpid());
    return (rand() % n);
}

void handlerSigterm(){

    int numero = generate_random_number();
    int pid = getpid();
    
    printf("Soy el proceso %d y el numero generado es: %d\n", pid, numero);

    if (numero == numero_maldito)
    {
        printf("Adios mundo cruel!\n");
        exit(EXIT_FAILURE);
    }
}

void handlerSigchld(){
    int pidMuerto = wait(NULL);
    
    for (size_t i = 0; i < n; i++)
    {
        if (pidHijos[i] == pidMuerto){
            pidHijos[i] = -2;
        }
    }
}

void procesoHijo(){
    signal(SIGTERM, (__sighandler_t) handlerSigterm);
    while(0<1){}
}

int main(int argc, char const *argv[]){
    
    n = atoi(argv[1]);              //Numero de procesos
	int rondas = atoi(argv[2]);     //Numero de rondas
	numero_maldito = atoi(argv[3]); //Numero maldito

    for (int i = 0; i < n; i++)
    {
        pidHijos[i] = -1;
    }

    //Creo los hijos y los hijos van al loop a esperar alguna se;al
    for (size_t i = 0; i < n; i++)
    {
        pidHijos[i] = fork();

        if (pidHijos[i] == 0)
        {
            procesoHijo();
        }
    }

    signal(SIGCHLD, (__sighandler_t) handlerSigchld);

    for (int i = 0; i < rondas; i++)
    {
        printf("Ronda %d\n",  i);
        
        //for (int j = 0; j < n; j++)
        //{
        //    printf("%d \n", pidHijos[j]);
        //}

        for (int j = 0; j < n; j++)
        {
            if (pidHijos[j] != -2)
            {
                sleep(1);
                //printf("ahhhh\n");
                kill(pidHijos[j], SIGTERM);
                //printf("hola\n");
            }
        }

        sleep(1);
    }

    for (size_t i = 0; i < n; i++)
    {
        if (pidHijos[i] != -2)
        {
            printf("El hijo de pid %d esta vivo\n", pidHijos[i]);
            kill(pidHijos[i], SIGKILL);
        }
    }

    return 0;
}