#include "kemmens/SocketServer.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <commons/string.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int sock = -1;

void SocketServer_Start(char name[5], t_log* logger, int port)
{
	//Guardar el nombre para logs.
	memcpy(alias, name, 5);

	struct sockaddr_in serv_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == -1)
	{
		log_error(logger, "Error al abrir socket del servidor!");
		exit_gracefully(EXIT_FAILURE);
	}

	log_info(logger, "Socket de servidor '%s' creado con exito.", alias);


	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if( bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		log_error(logger, "Error al bindear servidor '%s'!", alias);
		exit_gracefully(EXIT_FAILURE);
	}
}

void SocketServer_ListenForConnection(t_log* logger)
{
	if(sock < 0)
		return;

	if(listen(sock, MAXWAITCONNECTIONS) < 0)
	{
		log_error(logger, "'%s' -> Error al escuchar conexiones!", alias);
		exit_gracefully(EXIT_FAILURE);
	}

	log_info(logger, "Servidor '%s' escuchando por conexiones entrantes...", alias);

	connections = list_create();

	int connected_socket;

	struct sockaddr_in client;
	//while(1)
	{
		int c = sizeof(struct sockaddr_in);
		printf("1");
		connected_socket = accept(sock, (struct sockaddr*)&client, (socklen_t*)&c);
		printf("2");
		if(connected_socket < 0)
		{printf("3");
			log_error(logger, "'%s' -> Error al aceptar conexion entrante... skipeando conexion.", alias);
			//continue;
		}
		printf("4");
		close(sock);
		return;
		log_info(logger, "'%s' -> Conexion entrante aceptada en %d", connected_socket);
		printf("5");
		int* temp = malloc(sizeof(int));
		memcpy(temp, &connected_socket, sizeof(int));

		char* str = SocketCommons_ReceiveString(*temp);
		printf("Recibido: '%s'", str);
		free(str);

		list_add(connections, temp);
		sleep(1);
	}
	SocketServer_TerminateAllConnections(logger);
}

void SocketServer_TerminateAllConnections(t_log* logger)
{
	void cerrarconexion(void* conn)
	{
		log_info(logger, "'%s' -> Cerrando conexion %d ", (int)conn);

		if(close( *((int*)conn) ) < 0)
		{
			log_error(logger, "'%s' -> Error al cerrar una conexion!", alias);
		}
	}

	list_iterate(connections, cerrarconexion);
	list_destroy_and_destroy_elements(connections, free);
	close(sock);
}
