#include "kemmens/megekemmen.h"
#include "kemmens/SocketClient.h"

int SocketClient_ConnectToServer(char* port)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    // Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;  // Indica que usaremos el protocolo TCP

	getaddrinfo(SERVER_IP, port, &hints, &server_info);  // habria que poner una ip dinamica?

	// 2. Creemos el socket con el nombre "server_socket" usando la "server_info" que creamos anteriormente
	int server_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(server_socket == -1)
		return -1;

	// 3. Conectemosnos al server a traves del socket! Para eso vamos a usar connect()
	int retorno = connect(server_socket, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return ((retorno < 0) ? -1 : server_socket);
}
