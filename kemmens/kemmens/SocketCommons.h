#ifndef SOCKETCOMMONS_H_
#define SOCKETCOMMONS_H_

#include "SocketCommons.h"
#include "commons/string.h"


typedef struct {
  int body_length;
} __attribute__((packed)) ContentHeader;

int SocketCommons_SendMessageString(int socket, char* message);
int SocketCommons_ReceiveHeader(int socket);
ContentHeader* SocketCommons_CreateHeader();
char* SocketCommons_ReceiveString(int socket);

#endif /* SOCKETCOMMONS_H_ */
