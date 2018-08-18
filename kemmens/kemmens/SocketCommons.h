#ifndef SOCKETCOMMONS_H_
#define SOCKETCOMMONS_H_

#include "commons/string.h"
#include "commons/log.h"
#include "SocketMessageTypes.h"
#include <stdlib.h> // Para malloc
#include <sys/socket.h> // Para crear sockets, enviar, recibir, etc
#include <netdb.h> // Para getaddrinfo

typedef struct {
	uint32_t body_length;
	uint32_t message_type;
} __attribute__((packed)) ContentHeader;

/* Envia un string al socket indicado. Esta funcion ya envia el header automaticamente. Si el send del header falla devuelve -2.
 *
 *	retorna el estado del send, -2
 */
int SocketCommons_SendMessageString(t_log* log, int socket, char* message);

/* Recibe el header por el socket indicado.
 *
 *	retorna el socket propiamente dicho, en caso de error devuelve 0 (NULL). El free de la struct se hace solo en caso de NULL, sino hay que hacer el free.
 */
ContentHeader* SocketCommons_ReceiveHeader(t_log* log, int socket);

/*	Envia el header de datos, length es el tamaño de datos que va a recibir la otra parte, message_type es el tipo de mensaje a enviar (ver tipos en SocketMessageTypes.h)
 *
 * retorna el estado del send
 */
int SocketCommons_SendHeader(t_log* log, int socket, int length, int message_type);

/* Envia el header al socket, el length es el tamaño del mensaje que se enviara posterior al header.
 *
 *	retorna el estado del send;
 */
int SocketCommons_GetMessageLength(t_log* log, int socket);

/* Obtiene el length del body que se recibira.
 *
 *	retorna el length, -1 si hubo error.
 */
int SocketCommons_GetMessageLength(t_log* log, int socket);

/* Crea la estructura del header, se tiene que hacer free despues de usar.
 *
 *	retorna puntero a header.
 */
ContentHeader* SocketCommons_CreateHeader();

/* Recibe un string por el socket indicado. El tamaño lo toma automaticamenta llamando a ReceiveHeader() primero.
 *
 *	retorna el string. HACER FREE DESPUES DE USAR.
 */
char* SocketCommons_ReceiveString(t_log* log, int socket);

/* Recibe un string por el socket indicado. El tamaño se debe pasar por parametro.
 *
 *	retorna el string. HACER FREE DESPUES DE USAR.
 */
char* SocketCommons_ReceiveStringWithLength(t_log* log, int socket, int length);


/*		Recibe datos por el socket indicado. El tipo recibido se almacena en message_type. Los tipos soportados se pueden encontrar en SocketMessageTypes.h
 *
 *		retorna los datos recibidos. Hacer free del buffer despues de usar. Ojo de no hacer free si devuelve NULL.
 */
void* SocketCommons_ReceiveData(t_log* log, int socket, int* message_type);


/*		Envia datos por el socket indicado. El tipo puesto en message_type debe ser un tipo conocido y definido en SocketMessageTypes.h, data contiene los datos a ser enviado y dataLength es el largo de lo contenido en la variable data.
 *
 * 		retorna el estado de envio.
 */
int SocketCommons_SendData(t_log* log, int socket, int message_type, void* data, int dataLength);

#endif /* SOCKETCOMMONS_H_ */
