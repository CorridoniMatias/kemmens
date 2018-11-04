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
int closeAll = 0;
fd_set descriptores_clientes;

static ServerClient* SocketServer_CreateClient(int socketID)
{
	ServerClient* client = (ServerClient*)malloc(sizeof(ServerClient));

	client->socketID = socketID;
	client->isWaitingForData  = false;
	client->arriveData = NULL;
	sem_init(&client->waitForData, 0, 0);

	return client;
}

static void SocketServer_DestroyClient(void* client) //en realidad seria un ServerClient* pero asi cumple la interfaz de las commons
{
	ServerClient* rclient = (ServerClient*)client;
	sem_destroy(&rclient->waitForData);
	//El free de rclient->arriveData ya deberia haberlo hecho el thread cuando ejecuto.
	free(rclient);
}

void SocketServer_Start(char name[5], int port)
{
	//Guardar el nombre para logs.
	memcpy(alias, name, 5);

	struct sockaddr_in serv_addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == -1)
	{
		Logger_Log(LOG_ERROR, "Error al abrir socket del servidor!");
		exit_gracefully(EXIT_FAILURE);
	}

	Logger_Log(LOG_INFO, "Socket de servidor '%s' creado con exito.", alias);


	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	if( bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		Logger_Log(LOG_ERROR, "Error al bindear servidor '%s'!", alias);
		exit_gracefully(EXIT_FAILURE);
	}
}

void SocketServer_ListenForConnection(SocketServer_ActionsListeners actions)
{
	if(sock < 0)
		return;

	if(listen(sock, MAXWAITCONNECTIONS) < 0)
	{
		Logger_Log(LOG_ERROR,"'%s' -> Error al escuchar conexiones!", alias);
		exit_gracefully(EXIT_FAILURE);
	}

	Logger_Log(LOG_INFO, "Servidor '%s' escuchando por conexiones entrantes...", alias);

	connections = list_create();
	//ignoredSockets = list_create();
	pthread_mutex_init(&connections_lock, NULL);

	int connlistsize, currconn, connected_socket, sel, max_fd = sock; //el descriptor maximo empieza como sock (el master y unico que hay por ahora) pero puede ser mas grande a medida que lleguen sockets.
	ServerClient* currclient;
	//Si recibimos una funcion para manejar el procesamiento del input por linea se lo pasamos a readline.
	if(actions.OnConsoleInputReceived != 0)
		rl_callback_handler_install("> ", (rl_vcpfunc_t*) actions.OnConsoleInputReceived);


	while(1)
	{
		//limpiamos el set de descriptores.
		FD_ZERO(&descriptores_clientes);
		//agregamos el socket principal "sock" a los descriptores, asi si llega una nueva conexion podemos aceptarla desde el select. cool right?
		FD_SET(sock, &descriptores_clientes);
		FD_SET(STDIN, &descriptores_clientes);

		//Agregamos todos los descriptores a la lista de descriptores del select
		pthread_mutex_lock(&connections_lock);
		connlistsize = list_size(connections);
		for(int i = 0; i < connlistsize; i++)
		{
			//currconn = *((int*)list_get(connections, i)); //obtenemos el valor del socket como int almacenado en la lista.
			currclient = (ServerClient*)list_get(connections, i);
			FD_SET(currclient->socketID, &descriptores_clientes); //Registramos al cliente como un descriptor valido
		}
		pthread_mutex_unlock(&connections_lock);

		//Bloqueamos hasta que llegue algo.
		sel = select( max_fd + STDIN + 1 // la cantidad va a ser max_fd + STDIN + 1 porque todos los sockets que nos lleguen tanto del teclado como de red tienen que entrar
							, &descriptores_clientes // Le pasamos los descriptores.
							, NULL , NULL , NULL); //Queremos que nuestro select bloquee indefinidamente, sino le pasamos un timeout.

		/*
		 * La razon por la cual cada vez que empieza el while hay que volver a inicializar los descriptores es que
		 * cuando un cliente se va queda el socket abierto y como valido en la lista de FDs entonces el select siempre se va a desbloquear
		 * aunque no tenga que hacerlo, cuando recv devuelve 0 es porque se cerro un socket, ahi lo sacamos de nuestra lista personalizada
		 * y al comienzo del while volvemos a pasarle esa lista al select()
		 */

		if ((sel < 0) && (errno!=EINTR))
		{
			Logger_Log(LOG_ERROR, "'%s' -> Error en select = %d", alias, sel);
			continue;
		}

		//algo le llego al socket principal
		if (FD_ISSET(sock, &descriptores_clientes))
		{
			connected_socket = accept(sock, (struct sockaddr*)NULL, NULL);
			if(connected_socket < 0)
			{
				Logger_Log(LOG_ERROR, "'%s' -> Error al aceptar conexion entrante... skipeando conexion.", alias);
				continue;
			}

			Logger_Log(LOG_INFO, "'%s' -> Conexion entrante aceptada en %d", alias, connected_socket);

			ServerClient* client = SocketServer_CreateClient(connected_socket);
			pthread_mutex_lock(&connections_lock);
			list_add(connections, client);
			pthread_mutex_unlock(&connections_lock);
			/*
			int* temp = malloc(sizeof(int)); //el free a esto se hace cuando se cierra la conexion con este socket.
			memcpy(temp, &connected_socket, sizeof(int));
			list_add(connections, temp);
			*/
			if(actions.OnClientConnected != NULL)
				actions.OnClientConnected(connected_socket);

			if(max_fd < connected_socket)
				max_fd = connected_socket; //hay que actualizar el maximo valor por el cual va a escuchar el select, sino nunca va a escuchar el nuevo socket!
		}
		else
		{
			//int elemSize = list_size(ignoredSockets);
			//int tmp;
			//int isIgnored;
			//connlistsize se actualiza al comienzo del while
			for(int i = 0; i < connlistsize; i++)
			{
				pthread_mutex_lock(&connections_lock);
				//currconn = *((int*)list_get(connections, i)); //obtenemos el valor del socket como int almacenado en la lista.
				currclient = (ServerClient*)list_get(connections, i);
				pthread_mutex_unlock(&connections_lock);
				/*isIgnored = 0;

				for(int i = 0; i < elemSize; i++)
				{
					tmp = *((int*)list_get(ignoredSockets, i));
					if(tmp == currconn)
					{
						Logger_Log(LOG_DEBUG, "KEMMENSLIB -> SOCKETSERVER :: Ignorando elemento recibido por el socket %d, esta en la lista de ignorados!", tmp);
						isIgnored = 1;
						break;
					}
				}*/

				//LLego algo para el socket que estamos analizando en currconn
				if (FD_ISSET(currclient->socketID, &descriptores_clientes))
				{
					//NOS LLEGA UN MENSAJE DE UN CLIENTE!
					int message_type = -1;
					int error_code = -1;
					int message_length = -1;
					void* data;

					data = SocketCommons_ReceiveData(currclient->socketID, &message_type, &message_length, &error_code);

					if(data == 0) //recv devolvio 0 o sea nos cerraron el socket.
					{
						if(error_code == 0)
						{
							if(actions.OnClientDisconnect != NULL)
									actions.OnClientDisconnect(currclient->socketID);

							if(currclient->isWaitingForData)
							{
								currclient->arriveData = NULL;
								currclient->isWaitingForData = false;
								sem_post(&currclient->waitForData);
							}


							pthread_mutex_lock(&connections_lock);
							SocketServer_DestroyClient(list_remove(connections, i)); //Lo ultimo que hacemos es el free por si algun otro recurso hace referencia al espacio de memoria del int malloceado que representa al descriptor del socket.
							pthread_mutex_unlock(&connections_lock);


						} else
						{
							if(currclient->isWaitingForData)
							{
								currclient->arriveData = NULL;
								currclient->isWaitingForData = false;
								sem_post(&currclient->waitForData);
							}

							if(actions.OnReceiveError != NULL)
								actions.OnReceiveError(currclient->socketID, error_code); //error_code contiene el errno devuelto por el S.O, en el Log (si es que se inicializo) esta pasado a string, y tambien se puede obtener con strerror()
						}
					} else
					{
						//Si hay algun thread esperando datos en este socketID entonces no tenemos que llamar a OnPacketArrived, le derivamos la informacion al thread que la espera. Hablo en singular porque no tiene sentido que dos threads esten esperando en paralelo datos sobre EL MISMO socket.
						if(currclient->isWaitingForData)
						{
							Logger_Log(LOG_DEBUG, "KEMMENSLIB -> SOCKETSERVER :: Datos recibidos de %d, derivando al thread a la espera de los mismos. OnPacketArrived no sera llamado!", currclient->socketID);

							OnArrivedData* arriveData = SocketServer_CreateOnArrivedData();
							arriveData->calling_SocketID = currclient->socketID;
							arriveData->receivedData = data;
							arriveData->receivedDataLength = message_length;

							currclient->arriveData = arriveData;
							currclient->isWaitingForData = false;
							sem_post(&currclient->waitForData);
							break;
						}
						//OJO! Se debe hacer free(data) en la funcion que atienda la llegada del paquete.
						if(actions.OnPacketArrived != NULL)
								actions.OnPacketArrived(currclient->socketID,  message_type, data, message_length);
					}

					break;
				}
				currconn = 0;
				//Si recorrimos toda la lista y en ningun lado obtuvimos el socket que buscabamos entonces problablemente sea teclado, lo dejamos saber poniendo en -1 el flag.
				if(i == connlistsize-1)
				{
					currconn = -1;
				}

			}

			if((currconn == -1 || connlistsize == 0) && actions.OnConsoleInputReceived != NULL) //Input de teclado
					rl_callback_read_char(); //Readline nos hace el favor de ir acumulando los chars hasta recibir <enter>, ahi le manda la linea a consoleInputHandle.
		}
		if(closeAll == 1)
			break;
	}

	FD_ZERO(&descriptores_clientes);
	if(actions.OnConsoleInputReceived != NULL)
		rl_callback_handler_remove(); //Deregistrar el callback de readline.

	SocketServer_TerminateAllConnections();
	pthread_mutex_destroy(&connections_lock);
}

void SocketServer_TerminateAllConnections()
{
	void cerrarconexion(void* conn)
	{
		ServerClient* client = (ServerClient*)conn;

		if(client->isWaitingForData)
		{
			client->arriveData = NULL;
			client->isWaitingForData = false;
			sem_post(&client->waitForData);
		}

		Logger_Log(LOG_INFO, "'%s' -> Cerrando conexion %d ", alias, client->socketID);

		if(close( client->socketID ) < 0)
		{
			Logger_Log(LOG_ERROR, "'%s' -> Error al cerrar conexion %d!", alias, client->socketID);
		}
	}
	pthread_mutex_lock(&connections_lock);
	list_iterate(connections, cerrarconexion);
	list_destroy_and_destroy_elements(connections, (void*)SocketServer_DestroyClient);
	pthread_mutex_unlock(&connections_lock);
	//list_destroy_and_destroy_elements(ignoredSockets, (void*)free);
	close(sock);
}

bool SocketServer_IsClientConnected(int socket)
{
	bool connected(void* client)
	{
		return ((ServerClient*)client)->socketID == socket;
	}

	return list_any_satisfy(connections, connected);
}

void SocketServer_Stop()
{
	closeAll = 1;
}
/*
t_list* SocketServer_GetConnectedClients()
{
	return connections;
}

t_list* SocketServer_GetIgnoredSockets()
{
	return ignoredSockets;
}

void SocketServer_IgnoreSocket(int socketToIgnore)
{
	int* temp = malloc(sizeof(int));
	memcpy(temp, &socketToIgnore, sizeof(int));
	list_add(ignoredSockets, temp);
}

void SocketServer_ReAttendSocket(int socketToAttend)
{
	int elemSize = list_size(ignoredSockets);
	int tmp;
	for(int i = 0; i < elemSize; i++)
	{
		tmp = *((int*)list_get(ignoredSockets, i));

		if(tmp == socketToAttend)
			list_remove_and_destroy_element(ignoredSockets, i, (void*)free);
	}
}
*/
OnArrivedData* SocketServer_CreateOnArrivedData()
{
	return (OnArrivedData*)malloc(sizeof(OnArrivedData));
}

//Funcion thredeada
OnArrivedData* SocketServer_WakeMeUpWhenDataIsAvailableOn(int socketToWatch)
{
	bool match = false;
	pthread_mutex_lock(&connections_lock);
	int connlistsize = list_size(connections);
	ServerClient* currclient;
	for(int i = 0; i < connlistsize; i++)
	{
		currclient = (ServerClient*)list_get(connections, i);

		if(currclient->socketID == socketToWatch)
		{
			currclient->isWaitingForData = true;
			match = true;
			break;
		}
	}
	pthread_mutex_unlock(&connections_lock);

	if(match)
	{
		sem_wait(&currclient->waitForData);
		return currclient->arriveData;
	}

	return NULL;
}
