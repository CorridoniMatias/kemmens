#ifndef SOCKETCLIENT_H_
#define SOCKETCLIENT_H_

#define SERVER_IP "127.0.0.1"
#include <sys/socket.h> // Para crear sockets, enviar, recibir, etc
#include <netdb.h> // Para getaddrinfo
#include "SocketCommons.h"
#include <string.h>

int SocketClient_ConnectToServer(char* port);


#endif /* SOCKETCLIENT_H_ */
