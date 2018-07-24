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
#include <errno.h>
#include <readline/readline.h>

#define STDIN 0 //Input basico

int sock = -1;
fd_set descriptores_clientes;

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

void SocketServer_ListenForConnection(t_log* logger, void (*consoleInputHandle)())
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

	int connlistsize, currconn, connected_socket, sel, max_fd = sock; //el descriptor maximo empieza como sock (el master y unico que hay por ahora) pero puede ser mas grande a medida que lleguen sockets.

	//Si recibimos una funcion para manejar el procesamiento del input por linea se lo pasamos a readline.
	if(consoleInputHandle != NULL)
		rl_callback_handler_install("> ", (rl_vcpfunc_t*) consoleInputHandle);

	while(1)
	{

		//limpiamos el set de descriptores.
		FD_ZERO(&descriptores_clientes);

		//agregamos el socket principal "sock" a los descriptores, asi si llega una nueva conexion podemos aceptarla desde el select. cool right?
		FD_SET(sock, &descriptores_clientes);

		//Bloqueamos hasta que llegue algo.
		sel = select( max_fd + STDIN + 1 // la cantidad va a ser max_fd + STDIN + 1 porque todos los sockets que nos lleguen tanto del teclado como de red tienen que entrar
							, &descriptores_clientes // Le pasamos los descriptores.
							, NULL , NULL , NULL); //Queremos que nuestro select bloquee indefinidamente, sino le pasamos un timeout.

		if ((sel < 0) && (errno!=EINTR))
		{
			log_error(logger, "'%s' -> Error en select = %d", alias, sel);
			continue;
		}

		//algo le llego al socket principal
		if (FD_ISSET(sock, &descriptores_clientes))
		{
			connected_socket = accept(sock, (struct sockaddr*)NULL, NULL);
			if(connected_socket < 0)
			{
				log_error(logger, "'%s' -> Error al aceptar conexion entrante... skipeando conexion.", alias);
				continue;
			}

			log_info(logger, "'%s' -> Conexion entrante aceptada en %d", alias, connected_socket);

			int* temp = malloc(sizeof(int));
			memcpy(temp, &connected_socket, sizeof(int));
			list_add(connections, temp);

		}
		else
		{
			connlistsize = list_size(connections);
			for(int i = 0; i < connlistsize; i++)
			{
				currconn = *((int*)list_get(connections, i));
				if (FD_ISSET(currconn, &descriptores_clientes))
				{
					//NOS LLEGA UN MENSAJE DE UN CLIENTE!

					char* str = 0;
							//while(1)
							{
								str = SocketCommons_ReceiveString(*temp);
								printf("Recibido: '%s'\n", str);
								if(strcmp(str, "close") == 0)
									break;
								free(str);
							}


					break;
				}

				//Si recorrimos toda la lista y en ningun lado obtuvimos el socket que buscabamos entonces problablemente sea teclado, lo dejamos saber poniendo en -1 el flag.
				if(i == connlistsize-1)
					currconn = -1;
			}

			if(currconn == -1) //Input de teclado
				rl_callback_read_char(); //Readline nos hace el favor de ir acumulando los chars hasta recibir <enter>, ahi le manda la linea a consoleInputHandle.
		}

		sleep(1);
	}
	rl_callback_handler_remove(); //Deregistrar el callback de readline.
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
