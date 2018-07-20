#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include "megekemmen.h"
#include <commons/collections/list.h>
#include <commons/log.h>

#define MAXWAITCONNECTIONS 10

t_list* connections;

char alias[5];
void TerminateConnections(t_log* logger);
void ListenForConnection(t_log* logger);
void StartSocketServer(char name[5], t_log* logger, int port);


#endif
