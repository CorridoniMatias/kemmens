#include "kemmens/SocketCommons.h"

int SocketCommons_SendMessageString(int socket, char* message)
{
	return send(socket, message, string_length(message), 0);
}
