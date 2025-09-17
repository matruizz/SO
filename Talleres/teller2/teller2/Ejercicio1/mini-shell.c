#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "mini-shell-parser.c"

enum {READ, WRITE};

void first_child(int count, char ***progs, int pipes[][2]){

	int pid = getpid();
	printf("Soy el primer hijo!!!!!!!!!!!!%d\n", pid);

	close(pipes[0][READ]);
	
	dup2(pipes[0][WRITE], STDOUT_FILENO);

	for(int i = 1; i < count - 1; i++){
		close(pipes[i][WRITE]);
		close(pipes[i][READ]);
	}

	execvp(progs[0][0], progs[0]);

	//close(pipes[0][WRITE]);

	//printf("llegue hasta aca\n");

	//exit(EXIT_SUCCESS);
}

void last_child(int i, int count, char ***progs, int pipes[][2]){

	int pid = getpid();
	printf("Soy el ultimo hijo!!!!!!!!!!!!%d\n", pid);

	close(pipes[count - 2][WRITE]);

	dup2(pipes[i - 1][READ], STDIN_FILENO);

	for(int j = 0; j < count - 2; j++){
		close(pipes[j][WRITE]);
		close(pipes[j][READ]);
	}

	execvp(progs[count - 1][0], progs[count - 1]);

	//close(pipes[count - 1][READ]);

	//printf("llegue hasta aca\n");
	//exit(EXIT_SUCCESS);
}

void middle_child(int i, int count, char ***progs, int pipes[][2]){

	int pid = getpid();
	printf("Soy el hijo %d!!!!!!!!!!!!%d\n", i, pid);

	//Reservo memoria para el arreglo de pids
	//close(pipes[i - 1][WRITE]);
	//close(pipes[i][READ]);

	dup2(pipes[i - 1][READ], STDIN_FILENO);

	dup2(pipes[i][WRITE], STDOUT_FILENO);


	for(int j = 0; j < count - 1; j++){
		if(j != i-1){
			close(pipes[j][READ]);
		}

		if (j != i){
			close(pipes[j][WRITE]);
		}
	}

	execvp(progs[i][0], progs[i]);

	//close(pipes[i - 1][READ]);
	//close(pipes[i][READ]);

	//exit(EXIT_SUCCESS);
}



static int run(char ***progs, size_t count)
{	
	int r, status;

	//Reservo memoria para el arreglo de pids
	//TODO: Guardar el PID de cada proceso hijo creado en children[i]
	pid_t *children = malloc(sizeof(*children) * count);
	
	int pipes[count-1][2];
	// creamos matriz de pipes donde count es la cantidad de pipes que viene dada
	
	for(int i = 0; i < count -1; i ++){
		//creamos pipes en cada elementeo de la matriz
		if (pipe(pipes[i]) == -1){
			exit(EXIT_FAILURE);
		};
	}

	int pid = getpid();
	//printf("Soy el proceso padre!!!!!!!!!!!!%d\n", pid);
	
	//TODO: Pensar cuantos procesos necesito
	//TODO: Pensar cuantos pipes necesito.
 	for(int i = 0; i < count ; i++){
		children[i] = fork();
		if(children[i] == 0 ){
			if(i == 0){
				first_child(count, progs, pipes);
			} else if(i == count - 1){
				last_child(i,count, progs, pipes);
			} else {
				middle_child(i, count, progs, pipes);
			}
		}
	}

	


	for (int i = 0; i < count - 1; i++)
	{
		close(pipes[i][READ]);
		close(pipes[i][WRITE]);
		printf("cerrando %d\n", i);
	}
		
	printf("Soy el proceso padre!!!!!!!!!!!!%d\n", pid);




	//TODO: Para cada proceso hijo:
			//1. Redireccionar los file descriptors adecuados al proceso
			//2. Ejecutar el programa correspondiente


	//Espero a los hijos y verifico el estado que terminaron
	for (int i = 0; i < count; i++) {
		//kill(children[i], 9);
		waitpid(children[i], &status, 0);

		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}
	r = 0;
	free(children);

	printf("Soy el proceso padre!!!!!!!!!!!!%d\n", pid);

	return r;
	}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("El programa recibe como parametro de entrada un string con la linea de comandos a ejecutar. \n"); 
		printf("Por ejemplo ./mini-shell 'ls -a | grep anillo'\n");
		return 0;
	}
	int programs_count;
	char*** programs_with_parameters = parse_input(argv, &programs_count);

	printf("status: %d\n", run(programs_with_parameters, programs_count));

	fflush(stdout);
	fflush(stderr);

	return 0;
}
