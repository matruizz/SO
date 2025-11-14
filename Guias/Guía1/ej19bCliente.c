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

    int contador = 0;
	int valor;

	while (contador < 3)
	{
        printf("Cliente envia al servidor el valor %d\n", valor);
		send(server_socket, &valor, sizeof(valor), 0);
        
        recv(server_socket, &valor, sizeof(valor), 0);
        printf("Cliente recibe del servidor el valor %d\n", valor);

        contador++;
        valor++;
	}
	
    close(server_socket);


	exit(0);
}