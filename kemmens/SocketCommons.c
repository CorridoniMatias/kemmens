#include "kemmens/SocketCommons.h"
#include "stdio.h"

ContentHeader* SocketCommons_CreateHeader()
{
	return (ContentHeader*)malloc(sizeof(ContentHeader));
}

int SocketCommons_SendMessageString(int socket, char* message)
{
	int leng = string_length(message);
	int status = SocketCommons_SendHeader(socket, leng);
	if(status < 0)
		return -2;

	return send(socket, message, leng, 0);
}

ContentHeader* SocketCommons_ReceiveHeader(int socket)
{
	//Creamos el header que nos va a dar el lengh del tamaño de lo que sea que vamos a recibir posteriormente en el "body"
	ContentHeader * header = SocketCommons_CreateHeader();
	//Recibimos el Header, al ser un struct compartido, tanto el server como el client lo conocen y saben crearlo para informarle al otro el tamaño del mensaje que van a mandar.
	int ret = recv(socket, header, sizeof(ContentHeader), 0);
	if(ret < 1)
	{
		free(header);
		return 0;
	}

	return header;
}

int SocketCommons_GetMessageLength(int socket)
{
	ContentHeader * header = SocketCommons_ReceiveHeader(socket);

	if(header == 0)
		return -1;

	int length = header->body_length;

	free(header);

	return length;
}

int SocketCommons_SendHeader(int socket, int length)
{
	ContentHeader * header = SocketCommons_CreateHeader();
	header->body_length = length;
	int status = send(socket, header, sizeof(ContentHeader), MSG_WAITALL);

	free(header);

	return status;
}

char* SocketCommons_ReceiveString(int socket)
{
	int length = SocketCommons_GetMessageLength(socket);

	if(length < 1)
		return 0;

	char* str = (char*)malloc(length+1); //+1 porque le vamos a agregar el \0
	int status = recv(socket, str, length, MSG_WAITALL);

	if(status < 1)
	{
		free(str);
		return 0;
	}
	str[length] = '\0';
	return str;
}








