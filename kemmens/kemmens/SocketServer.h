#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include "megekemmen.h"
#include <string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include "SocketCommons.h"

#define MAXWAITCONNECTIONS 10

t_list* connections;

char alias[5];
void SocketServer_TerminateAllConnections(t_log* logger);
void SocketServer_ListenForConnection(t_log* logger);
void SocketServer_Start(char name[5], t_log* logger, int port);


#endif
