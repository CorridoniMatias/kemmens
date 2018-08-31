#ifndef SOCKETHANDLERSERVER_H
#define SOCKETHANDLERSERVER_H

#include "megekemmen.h"
#include <string.h>
#include <commons/collections/list.h>
#include "logger.h"
#include "SocketCommons.h"

#define MAXWAITCONNECTIONS 10

#define INIT_ACTION_LISTENER {0}

struct
{
	void (*OnClientConnected)(int socketID);
	void (*OnClientDisconnect)(int socketID);
	void (*OnReceiveError)(int socketID, int errorCode);
	void (*OnPacketArrived)(int socketID, int messageType, void* actualData);
	void (*OnConsoleInputReceived)(char* line);
} typedef SocketServer_ActionsListeners;

t_list* connections;

char alias[5];
void SocketServer_TerminateAllConnections();
void SocketServer_ListenForConnection(SocketServer_ActionsListeners actions);
void SocketServer_Start(char name[5], int port);
void SocketServer_Stop();
t_list* SocketServer_GetConnectedClients();


#endif
