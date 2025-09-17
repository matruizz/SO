#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int contador = 5;

void sigurgHandler(){	
	printf("Ya va!\n");
	contador--;
}

void sigintHandler(){
	wait(NULL);
	exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {

	/*
	¿Cuantos procesos se lanzan y que comportamiento se puede observar de cada uno?
	se lanzan 2 procesos el padre imprime "sup!" y el hijo imprime "ya va!", el padre modifica 
	el handler de la se;al sigint y el hijo el de la se;al sigurg, el padre se duerme 1 segundo y
	despues de imprimir "sup!" manda la se;al sigurg al hijo y eso parece hacer que el hijo 
	imprima un "ya va!"

	¿Utilizan los procesos alguna forma de IPC?¿Cu´al es y para qu´e se usa en este caso?
	Utilizan se;ales el padre usa la se;al sigurg para hacer que el hijo imprima ya va y el hijo utiliza 
	sigint para hacer que el padre lo espere

	¿Qu´e puede deducir del programa a partir de las syscalls observadas?
	que el programa hace un fork que crea a un proceso hijo, el proceso padre modifica el handler de la
	se;al sigint y el hijo el handler de la se;al sigurg, el padre se duerme 1 segundo imprime "sup!" y 
	manda la se;al sigurg al hijo que imprime ya va como consecuencia de recibir la se;a;l, este proceso
	se repite 5 veces, hasta que en la ultima iteracion el hijo pide el pid del padre y le manda la 
	se;al sigint, lo que hace que el padre espere a que el hijo termine su ejecucion por medio de un wait
	luego el hijo hace una llamada a execve para ejecutar el programa pasado como parametro.
	*/
  	
	//execvp(argv[1], &argv[1]);
	
	int pidHijo = fork();

	if (pidHijo == 0)
	{
		signal(SIGURG, (__sighandler_t) sigurgHandler);
		
		while (contador > 0){}
		
		int pidPadre = getppid();
		kill(pidPadre, SIGINT);

		execvp(argv[1], &argv[1]);
	}else{
		signal(SIGINT, (__sighandler_t) sigintHandler);
		while (0 < 1)
		{
			//signal(SIGCHLD, NULL);
			sleep(1);
			printf("Sup!\n");
			kill(pidHijo, SIGURG);
		}
	}
	

	return 0;
}
