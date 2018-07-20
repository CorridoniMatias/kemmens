#include "SocketServer.h"
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

void StartSocketServer(char name[5], t_log* logger)
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
	serv_addr.sin_port = htons(ConfigGeneral.ListenPort);

	if( bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		log_error(logger, "Error al bindear servidor '%s'!", alias);
		exit_gracefully(EXIT_FAILURE);
	}
}

void ListenForConnection(t_log* logger)
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

	while(1)
	{
		connected_socket = accept(sock, (struct sockaddr*)NULL, NULL);

		if(connected_socket < 0)
		{
			log_error(logger, "'%s' -> Error al aceptar conexion entrante... skipeando conexion.", alias);
			continue;
		}

		int* temp = malloc(sizeof(int));
		memcpy(temp, &connected_socket, sizeof(int));
		list_add(connections, temp);
		sleep(1);
	}
}

void TerminateConnections(t_log* logger)
{
	void cerrarconexion(void* conn)
	{
		if(close( *((int*)conn) ) < 0)
		{
			log_error(logger, "'%s' -> Error al cerrar una conexion!", alias);
		}
	}

	list_iterate(connections, cerrarconexion);
	list_destroy_and_destroy_elements(connections, free);
	close(sock);
}
