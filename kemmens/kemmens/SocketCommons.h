#ifndef SOCKETCOMMONS_H_
#define SOCKETCOMMONS_H_

#include "commons/string.h"
#include <string.h>
#include "logger.h"
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
int SocketCommons_SendMessageString(int socket, char* message);

/*
 * 		Permite enviar un string serializado indicando el tipo de dato serializado en serialized_content_type
 */
int SocketCommons_SendSerializedContent(int socket, char* serialized, int serialized_content_type);

/* Recibe el header por el socket indicado.
 *
 *	retorna el socket propiamente dicho, en caso de error devuelve 0 (NULL). El free de la struct se hace solo en caso de NULL, sino hay que hacer el free.
 */
ContentHeader* SocketCommons_ReceiveHeader(int socket, int* error_status);

/*	Envia el header de datos, length es el tamaño de datos que va a recibir la otra parte, message_type es el tipo de mensaje a enviar (ver tipos en SocketMessageTypes.h)
 *
 * retorna el estado del send
 */
int SocketCommons_SendHeader(int socket, int length, int message_type);

/* Crea la estructura del header, se tiene que hacer free despues de usar.
 *
 *	retorna puntero a header.
 */
ContentHeader* SocketCommons_CreateHeader();


/*		Recibe datos por el socket indicado. El tipo recibido se almacena en message_type. Los tipos soportados se pueden encontrar en SocketMessageTypes.h
 *
 *		retorna los datos recibidos. Hacer free del buffer despues de usar. Ojo de no hacer free si devuelve NULL.
 */
void* SocketCommons_ReceiveData(int socket, int* message_type, int* error_status);


/*		Envia datos por el socket indicado. El tipo puesto en message_type debe ser un tipo conocido y definido en SocketMessageTypes.h, data contiene los datos a ser enviado y dataLength es el largo de lo contenido en la variable data.
 *
 * 		retorna el estado de envio.
 */
int SocketCommons_SendData(int socket, int message_type, void* data, int dataLength);

#endif /* SOCKETCOMMONS_H_ */
