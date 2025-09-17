#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


int main() {
	// Completar

	int server_socket;
    struct sockaddr_un server_addr;

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "unix_socket");

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error");
        exit(1);
    }

    char expresion[5];
	int res;

	int scanRes = scanf("%s", expresion);
	printf("%d\n", scanRes);

	while(scanRes < 1){
		printf("Formato de expresion incorrecto\n");
		scanRes = scanf("%s", expresion);
		printf("%d\n", scanRes);
	}

	while (strncmp(expresion, "exit", 5) != 0)
	{

		send(server_socket, expresion, 5, 0);
    	recv(server_socket, &res, sizeof(int), 0);
    	printf("Cliente: recibí %d del servidor!\n", res);
		
		scanRes = scanf("%s", expresion);
		printf("%d\n", scanRes);
		while(scanRes < 1){
			printf("Formato de expresion incorrecto\n");
			scanRes = scanf("%s", expresion);
			printf("%d\n", scanRes);
	}
	}
	
    close(server_socket);


	exit(0);
}
