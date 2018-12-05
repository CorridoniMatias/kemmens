#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#include <sys/socket.h> // Para crear sockets, enviar, recibir, etc
#include <netdb.h> // Para getaddrinfo
#include <string.h>
#include "SocketCommons.h"

//Para deshabilitar el logueo interno de este modulo de las kemmens comentar la siguiente linea:
#define SCLI_ENABLE_LOGGING

/**
 * 		ACCION: Establece una conexion como cliente a un servidor con cierta IP, en cierto puerto
 * 		PARAMETROS:
 * 			ip: IP del servidor al cual quiero conectarme
 * 			port: Puerto de la IP a la cual quiero conectarme, en forma de cadena
 * 		RETORNO: El numero de fd del socket a traves del cual se conecto, o bien -1 en caso de error
 * 		(Nombre puesto en ingles asi Mati no me bardea :o)
 */
int SocketClient_ConnectToServerIP(char* ip, char* puerto);

#endif /* SOCKETCLIENT_H_ */
