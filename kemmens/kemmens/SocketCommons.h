#ifndef SOCKETCOMMONS_H_
#define SOCKETCOMMONS_H_

#include "commons/string.h"
#include <stdlib.h> // Para malloc
#include <sys/socket.h> // Para crear sockets, enviar, recibir, etc
#include <netdb.h> // Para getaddrinfo


typedef struct {
  int body_length;
} __attribute__((packed)) ContentHeader;

/* Envia un string al socket indicado. Esta funcion ya envia el header automaticamente. Si el send del header falla devuelve -2.
 *
 *	retorna el estado del send, -2 en caso que falle el envio del header
 */
int SocketCommons_SendMessageString(int socket, char* message);

/* Recibe el header por el socket indicado.
 *
 *	retorna el socket propiamente dicho, en caso de error devuelve 0 (NULL). El free de la struct se hace solo en caso de NULL, sino hay que hacer el free.
 */
ContentHeader* SocketCommons_ReceiveHeader(int socket);

/* Envia el header al socket, el length es el tamaño del mensaje que se enviara posterior al header.
 *
 *	retorna el estado del send;
 */
int SocketCommons_SendHeader(int socket, int length);

/* Obtiene el length del body que se recibira.
 *
 *	retorna el length, -1 si hubo error.
 */
int SocketCommons_GetMessageLength(int socket);

/* Crea la estructura del header, se tiene que hacer free despues de usar.
 *
 *	retorna puntero a header.
 */
ContentHeader* SocketCommons_CreateHeader();

/* Recibe un string por el socket indicado. El tamaño lo toma automaticamenta llamando a ReceiveHeader() primero.
 *
 *	retorna el string. HACER FREE DESPUES DE USAR.
 */
char* SocketCommons_ReceiveString(int socket);

#endif /* SOCKETCOMMONS_H_ */
