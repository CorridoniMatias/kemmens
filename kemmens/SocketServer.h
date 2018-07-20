#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>

#define MAXWAITCONNECTIONS 10

t_list* connections;

char alias[5];
void TerminateConnections(t_log*);
void ListenForConnection(t_log*);
void StartSocketServer(char*, t_log*);


#endif
