#ifndef SOCKETHANDLERSERVER_H
#define SOCKETHANDLERSERVER_H

#include "megekemmen.h"
#include <string.h>
#include <commons/collections/list.h>
#include "logger.h"
#include "SocketCommons.h"
#include <semaphore.h>
#include <pthread.h>

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
	 */
	void (*OnPacketArrived)(int socketID, int messageType, void* actualData);
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
	void* receivedData;
} typedef OnArrivedData;

struct
{
	int socketID; 			//Or the socket file descriptor
	sem_t waitForData;		//Semaphore used when: another thread is expecting to receive data on the socket described by 'socketID'. If this occurs the thread should call SocketServer_WakeMeUpWhenDataIsAvailableOn(<socketID>).
	bool isWaitingForData;	//If another thread if expecting to receive data on the socket described by 'socketID' this attribute is set to true, false otherwise.
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
 * 		ACCION: Crea el servidor, definiendo el socket y bindeandolo a la conexion especificada
 * 		PARAMETROS:
 * 			name: Nombre del servidor, en forma de cadena (no mas de 4 caracteres)
 * 			port: Puerto a traves del cual quiero escuchar
 * 			ip: Ip del servidor que queremos iniciar
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
 * 		!!!! Funcion a ser llamada por un thread DISTINTO al que esta corriendo el server (donde se llamo a listenForConnections) !!!!
 *
 * 		Le indica al Server que cuando haya datos disponibles en el socket 'socketToWatch' no haga el flujo normal de llamar a OnPacketReceived()
 * 		sino que va a desbloquear el hilo que llamo a esta funcion.
 *
 * 		SOLAMENTE DEBE SER LLAMARA POR UN HILO A LA VEZ!
 */
bool SocketServer_WakeMeUpWhenDataIsAvailableOn(int socketToWatch);

/*
 *		Le avisa al servidor que el socket 'disconnectedSocket' se desconecto para que este lo remueva de la lista de clientes.
 *
 *		Si un cliente se desconecta y el server se entera por este medio (el recv estaba siendo manejado por otro thread, por ejemplo) entonces OnClientDisconnect no sera llamado.
 */
void SocketServer_NotifyClientDisconnect(int disconnectedSocket);

/*
t_list* SocketServer_GetIgnoredSockets();

void SocketServer_IgnoreSocket(int socketToIgnore);

void SocketServer_ReAttendSocket(int socketToAttend);
*/

#endif
