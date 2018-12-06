#ifndef SOCKETHANDLERSERVER_H
#define SOCKETHANDLERSERVER_H

#include "megekemmen.h"
#include <string.h>
#include <commons/collections/list.h>
#include "logger.h"
#include "SocketCommons.h"
#include <semaphore.h>
#include <pthread.h>
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

//Para deshabilitar el logueo interno de este modulo de las kemmens descomentar la siguiente linea:
//#define SOCKETSERVER_DISABLE_LOGGING

#define MAXWAITCONNECTIONS 10

#define INIT_ACTION_LISTENER {0}


/////---------ESTRUCTURAS Y VARIABLES GLOBALES---------/////


/**
 * 		Estructura que almacena distintas funciones, listeners, a realizar segun ocurran ciertos eventos
 */
struct
{
	/**
	 * Accion a realizar al conectarse un cliente
	 * PARAMETROS:
	 * 		socketID: Socket a traves del cual se comunica el cliente recien conectado
	*/
	void (*OnClientConnected)(int socketID);
	/**
	 * Accion a realizar al desconectarse un cliente
     * PARAMETROS:
     * 		socketID: Socket a traves del cual se comunicaba el cliente recien desconectado
	 */
	void (*OnClientDisconnect)(int socketID);
	/**
	 * Accion a realizar al producirse un error
	 * PARAMETROS:
	 * 		socketID: Socket a traves del cual llego el error
	 * 		errorCode: Codigo de error recibido
	 */
	void (*OnReceiveError)(int socketID, int errorCode);
	/**
	 * Accion a realizar ante la recepcion de un paquete de informacion
	 * PARAMETROS:
	 * 		socketID: Socket a traves del cual arribo el paquete
	 * 		messageType: Tipo de mensaje recibido; ver tipos en SocketMessageTypes.h
	 * 		actualData: Paquete de datos recibido
	 * 		actualDataLength: Tamaño del contenido de actualData.
	 */
	void (*OnPacketArrived)(int socketID, int messageType, void* actualData, int actualDataLength);
	/**
	 * Accion a realizar al recibir una entrada por consola
	 * PARAMETROS:
	 * 		line: Comando ingresado por consola, en forma de cadena
	 */
	void (*OnConsoleInputReceived)(char* line);
} typedef SocketServer_ActionsListeners;

struct
{
	int calling_SocketID;
	int receivedDataLength;
	int message_type;
	void* receivedData;
} typedef OnArrivedData;

struct
{
	int socketID; 				//Or the socket file descriptor
	sem_t waitForData;			//Semaphore used when: another thread is expecting to receive data on the socket described by 'socketID'. If this occurs the thread should call SocketServer_WakeMeUpWhenDataIsAvailableOn(<socketID>).
	bool isWaitingForData;		//If another thread if expecting to receive data on the socket described by 'socketID' this attribute is set to true, false otherwise.
	OnArrivedData* arriveData;	//When the socketserver reads (recv()) the content the thread is expecting, its content and other data is stored in this heap variable.
} typedef ServerClient;


/**
 * 		Lista de conexiones activas del servidor
 */
t_list* connections;
pthread_mutex_t connections_lock;

/*
 * 		Lista de descriptores de sockets que el select va a ignorar, esto puede ser porque su procesamiento se este haciendo en otro lado.
 */
//t_list* ignoredSockets;

/**
 * Cadena que almacena el nombre del servidor, para volcar en los logs
 */
char alias[5];


/////---------FUNCIONES DEFINIDAS---------/////


/**
 * 		ACCION: Cerrar todas las conexiones de connections, logear el
 * 		resultado de cada clausura, destruir la lista y cerrar el socket escucha
 */
void SocketServer_TerminateAllConnections();

/**
 * 		ACCION: Pone a escuchar al socket, crea la lista connections y se queda multiplexando a la espera de conexiones
 * 		PARAMETROS:
 * 			actions: Estructura con acciones a tomar ante cada evento
 */
void SocketServer_ListenForConnection(SocketServer_ActionsListeners actions);

/**
 * 		ACCION: Crea el servidor, definiendo el socket y bindeandolo al puerto especificado
 * 		PARAMETROS:
 * 			name: Nombre del servidor, en forma de cadena (no mas de 4 caracteres)
 * 			port: Puerto a traves del cual quiero escuchar
 */
void SocketServer_Start(char name[5],int port);

/**
 * 		ACCION: Pone el flag de clausura en 1 (indicando que es propicio cerrar toda conexion)
 */
void SocketServer_Stop();

/**
 * 		ACCION: Devuelve todos los clientes conectados, es decir, la lista connections
 */
t_list* SocketServer_GetConnectedClients();

/*
 * 		Comprueba si un socket todavia esta conectado como cliente.
 *
 * 		ES FUNDAMENTAL CHECKEAR ESTO ANTES DE HACER UN SEND!
 */
bool SocketServer_IsClientConnected(int socket);


/**
 * 		Mallocea la estructura del tipo que devuelve.
 */
OnArrivedData* SocketServer_CreateOnArrivedData();

/*
 * 		Libera correctamente la memoria ocupada por un OnArrivedData, haciendo free del receivedData
 * 		y del OnArrivedData.
 */

void SocketServer_CleanOnArrivedData(OnArrivedData* data);

/*
 * 		!!!! Funcion a ser llamada por un thread DISTINTO al que esta corriendo el server (donde se llamo a listenForConnections) !!!!
 *
 * 		Le indica al Server que cuando haya datos disponibles en el socket 'socketToWatch' no haga el flujo normal de llamar a OnPacketReceived()
 * 		sino que va a desbloquear el hilo que llamo a esta funcion.
 *
 * 		Retorna un OnArriveData con todo el contenido recibido, verificar la estructura para ver que datos estan disponibles.
 *
 * 		Si el valor retornado es NULL, entonces puede ser que haya habido un error al recibir (ver OnReceiveError),
 * 			el cliente se desconecto (OnClientDisconnect) o que el socket que se pidio vigilar no esta en la lista de clientes.
 *
 * 		!! IMPORTANTE:
 *
 * 			- Hacer free de la data recibida y de la estructura cuando se termina de usar!
 *
 *			- NO LLAMAR DESDE EL MISMO HILO QUE SE HIZO SocketServer_ListenForConnection!
 *
 * 			- SOLAMENTE DEBE SER LLAMARA POR UN HILO A LA VEZ!
 */
OnArrivedData* SocketServer_WakeMeUpWhenDataIsAvailableOn(int socketToWatch);


/*
t_list* SocketServer_GetIgnoredSockets();

void SocketServer_IgnoreSocket(int socketToIgnore);

void SocketServer_ReAttendSocket(int socketToAttend);
*/

#endif
