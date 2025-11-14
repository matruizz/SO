#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "constants.h"

enum {READ, WRITE};
int n;
int pipe_hijo_elegido_a_padre[2];
int pipe_padre_a_hijo_elegido[2];

int generate_random_number(){
	return (rand() % 50);
}

void hijo(int i, int pipes[][2], int soy_elejido){
	int mensaje_recibido;

	for(int j = 0; j < n; j++){
		if(j != (n + i-1) % n){
			close(pipes[j][READ]);
		}

		if (j != i){
			close(pipes[j][WRITE]);
		}
	}

	//si soy el elejido creo el numero secreto
	if (soy_elejido){
		int ronda = 0;
		int numero_secreto = generate_random_number();

		printf("El numero secreto es %d\n", numero_secreto);

		//cierro pipes que no son para mí.
		close(pipe_padre_a_hijo_elegido[WRITE]);
		close(pipe_hijo_elegido_a_padre[READ]);

		//printf("(i+n)  n %d\n", ((i+n) % n));
		//printf("(n+i-1)  n %d\n", (n+i-1) % n);
		//leo el primer mensaje
		if(read(pipe_padre_a_hijo_elegido[READ], &mensaje_recibido, sizeof(int)) < sizeof(int)){
			printf("Ocurrio un error al leer el pipe padre a hijo\n");
			exit(EXIT_SUCCESS);
		}

		printf("Ronda %d\n", ronda);
		printf("Soy el hijo %d y recibi %d\n", i, mensaje_recibido);

		if (mensaje_recibido >= numero_secreto){	//si me dieron algo mayor 
			//	lo envio al padre
			//printf("res %d\n", mensaje_recibido);

			if(write(pipe_hijo_elegido_a_padre[WRITE], &mensaje_recibido, sizeof(int)) < sizeof(int)){
				printf("Ocurrio un error al escribir en el pipe hijo elegido a padre\n");
			}

			close(pipe_hijo_elegido_a_padre[WRITE]);
			close(pipe_padre_a_hijo_elegido[READ]);
			close(pipes[(i+n) % n][WRITE]);
			close(pipes[(n+i-1) % n][READ]);
			exit(EXIT_SUCCESS);
		}

		ronda++;

		mensaje_recibido++;

		if(write(pipes[(i+n) % n][WRITE], &mensaje_recibido, sizeof(int)) < sizeof(int)){
			printf("ocurrio un error al escribir en el pipe %d\n", i);
		}
		
		//me pongo a leer lo que me dan
		while(1){
			if(read(pipes[(n+i-1) % n][READ], &mensaje_recibido, sizeof(int)) < sizeof(int)){
				printf("Ocurrio un error al leer el pipe %d\n", i-1);
			}
			printf("Ronda %d\n", ronda);
			printf("Soy el hijo %d y recibi %d\n", i, mensaje_recibido);

			if (mensaje_recibido >= numero_secreto){		//si me dieron algo mayor 
				//	lo envio al padre

				//printf("res %d\n", mensaje_recibido);

				if(write(pipe_hijo_elegido_a_padre[WRITE], &mensaje_recibido, sizeof(int)) < sizeof(int)){
					printf("Ocurrio un error al escribir en el pipe hijo elegido a padre\n");
				}
				close(pipe_hijo_elegido_a_padre[WRITE]);
				close(pipe_padre_a_hijo_elegido[READ]);
				close(pipes[(i+n) % n][WRITE]);
				close(pipes[(n+i-1) % n][READ]);
				exit(EXIT_SUCCESS);
			}
			else{
				ronda++;

				mensaje_recibido++;

				if(write(pipes[(n+i) % n][WRITE], &mensaje_recibido, sizeof(int)) < sizeof(int)){
					printf("ocurrio un error al escribir en el pipe %d", i);
				}
			}
		}
	}
	else{
		//cierro pipes que no son para mí.
		close(pipe_padre_a_hijo_elegido[WRITE]);
		close(pipe_padre_a_hijo_elegido[READ]);
		close(pipe_hijo_elegido_a_padre[READ]);
		close(pipe_hijo_elegido_a_padre[WRITE]);

		while(1){
			if (read(pipes[(n+i-1) % n][READ], &mensaje_recibido,sizeof(int)) == 0){
				close(pipes[(i+n) % n][WRITE]);
				close(pipes[(n+i-1) % n][READ]);
				exit(EXIT_SUCCESS);
			}

			printf("Soy el hijo %d y recibi %d\n", i, mensaje_recibido);

			mensaje_recibido++;

			if(write(pipes[(i+n) % n][WRITE], &mensaje_recibido, sizeof(int)) < sizeof(int)){
				printf("ocurrio un error al escribir en el pipe %d\n", i);
			}
		}
		
	}

}

int main(int argc, char **argv)
{	
	//Funcion para cargar nueva semilla para el numero aleatorio
	srand(time(NULL));

	int status, pid, hijo_elejido, valor_inicial;
	n = atoi(argv[1]);
	valor_inicial = atoi(argv[2]); //valor inicial del mensaje
	hijo_elejido = atoi(argv[3]); //hijo_elejido es el hijo elejido o sea s

	if (argc != 4){ printf("Uso: anillo <n> <c> <s> \n"); exit(0);}
    
    //printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, valor_inicial, hijo_elejido);
    
	int pipes[n][2];
	int children[n];

	for (int i = 0; i < n; i++){
		if(pipe(pipes[i]) != 0){
			printf("Ocurrio un error al inicializar el pipe %d\n", i);
			exit(EXIT_FAILURE);
		}
	}

	//me hago pipes con mi hijo elejido para pasarle el mensaje inicial y que me pase el ultimo
	if(pipe(pipe_padre_a_hijo_elegido) != 0){
			printf("Ocurrio un error al inicializar el pipe padre a hijo elegido\n");
			exit(EXIT_FAILURE);
	}
	if(pipe(pipe_hijo_elegido_a_padre) != 0){
		printf("Ocurrio un error al inicializar el pipe\n");
		exit(EXIT_FAILURE);
	}
	
	for (int i = 0; i < n; i++)
	{
		children[i] = fork();
		if (children[i] == 0){
			//printf("i: %d, hijo_elejido : %d\n", i, hijo_elejido);
			hijo(i, pipes, (i == hijo_elejido));
		}
	}

	for (int i = 0; i < n; i++){
		close(pipes[i][READ]);
		close(pipes[i][WRITE]);
	}


	close(pipe_padre_a_hijo_elegido[READ]);
	close(pipe_hijo_elegido_a_padre[WRITE]);

	if(write(pipe_padre_a_hijo_elegido[WRITE], &valor_inicial, sizeof(int)) < sizeof(int)){
		printf("ocurrio un error al escribir en el pipe padre a hijo elegido\n");
	}

	int mensaje_final;
	if(read(pipe_hijo_elegido_a_padre[READ], &mensaje_final, sizeof(int)) < sizeof(int)){
		printf("Ocurrio un error al leer el pipe hijo elegido a padre\n");
	}
	printf("El resultado final es %d\n",mensaje_final);

	for (size_t i = 0; i < n; i++)
	{
		wait(NULL);
	}
	

	//cierro todos
	close(pipe_padre_a_hijo_elegido[WRITE]);
	close(pipe_hijo_elegido_a_padre[READ]);
}
