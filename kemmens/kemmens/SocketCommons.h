#ifndef SOCKETCOMMONS_H_
#define SOCKETCOMMONS_H_

#include "commons/string.h"
#include <string.h>
#include <ctype.h>
#include "logger.h"
#include "SocketMessageTypes.h"
#include <stdlib.h> // Para malloc
#include <sys/socket.h> // Para crear sockets, enviar, recibir, etc
#include <netdb.h> // Para getaddrinfo

//Para deshabilitar el logueo interno de este modulo de las kemmens comentar la siguiente linea:
#define SCOM_ENABLE_LOGGING

/**
 * 		Estructura que representa el encabezado de un mensaje, para ser interpretada segun el protocolo
 * 		CAMPOS:
 * 			body_length: Largo del mensaje, en bytes
 * 			message_type: Tipo de mensaje a enviar/recibir; ver en SocketMessageTypes.h
 */
typedef struct {
	uint32_t body_length;
	uint32_t message_type;
} __attribute__((packed)) ContentHeader;

//------FUNCIONES DEFINIDAS------//

/* Envia un string al socket indicado. Esta funcion ya envia el header automaticamente. Si el send del header falla devuelve -2.
 *
 *	retorna el estado del send, -2
 */
int SocketCommons_SendMessageString(int socket, char* message);

/*
 * 		Permite enviar un string indicando el tipo de dato que representa en content_type
 */
int SocketCommons_SendStringAsContent(int socket, char* string, int content_type);

/*		Recibe datos por el socket indicado.
 *
 * 		El tipo recibido se almacena en message_type. Los tipos soportados se pueden encontrar en SocketMessageTypes.h
 *
 * 		El largo del contenido recibido se almacena en message_length.
 *
 * 		Si ocurre algun error se almacena en error_status. Este valor es lo que contenia errno en el momento del error.
 *
 *		Retorna los datos recibidos. Hacer free del buffer despues de usar. Ojo de no hacer free si devuelve NULL.
 */
void* SocketCommons_ReceiveData(int socket, int* message_type, int* message_length, int* error_status);

/*		Envia datos por el socket indicado. El tipo puesto en message_type debe ser un tipo conocido y definido en SocketMessageTypes.h, data contiene los datos a ser enviado y dataLength es el largo de lo contenido en la variable data.
 *
 * 		retorna el estado de envio.
 */
int SocketCommons_SendData(int socket, int message_type, void* data, int dataLength);

/*
 * 	ACCION: Cierra un socket; loguea el exito o fracaso en la clausura
 * 	PARAMETROS:
 * 		descriptorSocket: Descriptor numerico o fd del socket a cerrar
 */
void SocketCommons_CloseSocket(int descriptorSocket);

/* 	FUNCION PRIVADA/STATIC
 *
 * Recibe el header por el socket indicado.
 *
 *	retorna el socket propiamente dicho, en caso de error devuelve 0 (NULL). El free de la struct se hace solo en caso de NULL, sino hay que hacer el free.
 */
//ContentHeader* SocketCommons_ReceiveHeader(int socket, int* error_status);

/*	FUNCION PRIVADA/STATIC
 *
 * Envia el header de datos, length es el tamaño de datos que va a recibir la otra parte, message_type es el tipo de mensaje a enviar (ver tipos en SocketMessageTypes.h)
 *
 * retorna el estado del send
 */
//int SocketCommons_SendHeader(int socket, int length, int message_type);

/* FUNCION PRIVADA/STATIC
 *
 * Crea la estructura del header, se tiene que hacer free despues de usar.
 *
 *	retorna puntero a header.
 */
//ContentHeader* SocketCommons_CreateHeader();


/*
 * 		FUNCION PRIVADA/STATIC USAR SIEMPRE FUNCIONES CON HEADER
 *
 * 		Recibe datos por el socket indicado. A diferencia de SocketCommons_ReceiveData esta funcion NO espera a recibir primero un ContentHeader
 * 		por ese motivo se debe especificar a mano el expected_size.
 *
 * 		WARNING: el syscall a recv que se hace en esta funcion es BLOQUEANTE por ende no se va a DESBLOQUEAR hasta que se reciban los expected_size bytes.
 *
 * 		Uso recomendado: cuando se sabe el tamaño de lo que se va a recibir a priori y no se necesita especificar el tipo de datos tampoco.
 */
//void* SocketCommons_ReceiveDataWithoutHeader(int socket, int expected_size, int* error_status);

/*
 * 		FUNCION PRIVADA/STATIC USAR SIEMPRE FUNCIONES CON HEADER
 *
 *
 * 		Envia datos por el socket indicado. A diferencia de SocketCommons_SendData esta funcion NO envia el ContentHeader indicador de tamaño y tipo.
 *
 * 		Uso recomendado: cuando el recipiente ya sabe el tamaño de lo que va a recibir y tambien sabe el tipo de datos a recibir.
 */
//int SocketCommons_SendDataWithoutHeader(int socket, void* data, int dataLength);



#endif /* SOCKETCOMMONS_H_ */
