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

void procesoHijo(){
    signal(SIGTERM, (__sighandler_t) handlerSigterm);
    while(0<1){}
}

int main(int argc, char const *argv[]){
    
    n = atoi(argv[1]);              //Numero de procesos
	int rondas = atoi(argv[2]);     //Numero de rondas
	numero_maldito = atoi(argv[3]); //Numero maldito

    int pidHijos[n];

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
                kill(pidHijos[j], SIGTERM);
            }

            /*if ((i == 0) && (j == n-2))
            {
                sleep(1);
                kill(pidHijos[j+1], SIGTERM);
            }*/
        }
        sleep(1);
    }

    for (int j = 0; j < n; j++)
    {
        printf("%d \n", pidHijos[j]);
    }
    
    for (int i = 0; i < n; i++)
    {

        /*Waitpid retorna el pid del proceso que termino en caso de exito, pero si wnohang fue especificado 
        entonces retorna 0 si es que el proceso con ese pid existe pero todavia no termino
        si wstatus no es nulo entonces wait y waitpid guardan informacion de estado en el entero al que apuntan*/
        int res = waitpid(pidHijos[i], NULL, WNOHANG);

        if((res != 0) && (res != -1)){ /*Si el res es el pid del proceso al que espero y */
            printf("El hijo de identificador %d y pid %d murio\n", i, res);
        }else{
            printf("El hijo de identificador %d y de pid %d sobrevivio\n", i, pidHijos[i]);
            kill(pidHijos[i], SIGKILL);
        }
    }
    

    exit(0);
}