#include "kemmens/SocketCommons.h"
#include "stdio.h"

ContentHeader* SocketCommons_CreateHeader()
{
	return (ContentHeader*)malloc(sizeof(ContentHeader));
}

int SocketCommons_SendMessageString(int socket, char* message)
{
	int leng = string_length(message);
	return SocketCommons_SendData(socket, MESSAGETYPE_STRING, message, leng);
}

ContentHeader* SocketCommons_ReceiveHeader(int socket)
{
	//Creamos el header que nos va a dar el lengh del tamaño de lo que sea que vamos a recibir posteriormente en el "body"
	ContentHeader * header = SocketCommons_CreateHeader();
	//Recibimos el Header, al ser un struct compartido, tanto el server como el client lo conocen y saben crearlo para informarle al otro el tamaño del mensaje que van a mandar.
	int ret = recv(socket, header, sizeof(ContentHeader), 0);

	if(ret < 1)
	{
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveHeader - Error al recibir header, codigo: %d", ret);
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

int SocketCommons_SendHeader(int socket, int length, int message_type)
{
	ContentHeader * header = SocketCommons_CreateHeader();
	header->body_length = length;
	header->message_type = message_type;
	int status = send(socket, header, sizeof(ContentHeader), MSG_WAITALL);

	if(status < 0)
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_SendHeader - Error al enviar header, codigo: %d", status);

	free(header);

	return status;
}

char* SocketCommons_ReceiveString(int socket)
{
	int length = SocketCommons_GetMessageLength(socket);

	return SocketCommons_ReceiveStringWithLength(socket, length);
}

char* SocketCommons_ReceiveStringWithLength(int socket, int length)
{
	if(length < 1)
		return 0;

	char* str = (char*)malloc(length+1); //+1 porque le vamos a agregar el \0
	int status = recv(socket, str, length, MSG_WAITALL);

	if(status < 1)
	{
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveStringWithLength - Error al recibir string, codigo: %d", status);
		free(str);
		return 0;
	}
	str[length] = '\0';
	return str;
}

void* SocketCommons_ReceiveData(int socket, int* message_type)
{
	ContentHeader* header = SocketCommons_ReceiveHeader(socket);

	if(header == 0)
		return 0;

	int len = header->body_length;
	//Informamos al usuario de la funcion que es lo que vamos a recibir para que despues puedan castear nuestro void*
	*message_type = header->message_type;
	free(header);

	Logger_Log((void*)log_info, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveData - Recibiendo datos, length: %d, content type: %d", len, *message_type);

	//los strings necesitan un manejo especial con el \0 por eso hacemos esto.
	if(*message_type == MESSAGETYPE_STRING)
	{
		return SocketCommons_ReceiveStringWithLength(socket, len);
	}

	void* buffer = malloc(len);

	int status = recv(socket, buffer, len, MSG_WAITALL);

	if(status < 1)
	{
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveData - Error al recibir datos, codigo: %d", status);
		free(buffer);
		return 0;
	}

	return buffer;
}

int SocketCommons_SendData(int socket, int message_type, void* data, int dataLength)
{
	int status = SocketCommons_SendHeader(socket, dataLength, message_type);

	if(status < 0)
		return -2;

	status = send(socket, data, dataLength, MSG_WAITALL);

	if(status < 0)
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_SendData - Error al enviar data, codigo: %d", status);

	return status;
}








