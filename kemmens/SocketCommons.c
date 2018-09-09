#include "kemmens/SocketCommons.h"
#include <errno.h>
#include "stdio.h"
#include <unistd.h>

ContentHeader* SocketCommons_CreateHeader()
{
	return (ContentHeader*)malloc(sizeof(ContentHeader));
}

int SocketCommons_SendMessageString(int socket, char* message)
{
	return SocketCommons_SendSerializedContent(socket, message, MESSAGETYPE_STRING);
}

int SocketCommons_SendSerializedContent(int socket, char* serialized, int serialized_content_type)
{
	int leng = string_length(serialized) + 1;
	return SocketCommons_SendData(socket, serialized_content_type, serialized, leng);
}

ContentHeader* SocketCommons_ReceiveHeader(int socket, int* error_status)
{
	//Creamos el header que nos va a dar el lengh del tamaño de lo que sea que vamos a recibir posteriormente en el "body"
	ContentHeader * header = SocketCommons_CreateHeader();
	//Recibimos el Header, al ser un struct compartido, tanto el server como el client lo conocen y saben crearlo para informarle al otro el tamaño del mensaje que van a mandar.
	int ret = recv(socket, header, sizeof(ContentHeader), 0);

	if(ret < 1)
	{
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveHeader - Error al recibir header, return recv: %d", ret);
		free(header);

		if(error_status != 0)
		{
			if(ret < 0)
			{
				*error_status = errno;
				Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveHeader - Error al recibir header, errno = %d = '%s'", *error_status, strerror(*error_status));
			}
			else
				*error_status = 0;//En general SIEMPRE lo primero que recibimos es el header, por ende aca es donde detectamos si el cliente se desconecto.
		}

		return NULL;
	}

	return header;
}

int SocketCommons_SendHeader(int socket, int length, int message_type)
{
	ContentHeader * header = SocketCommons_CreateHeader();
	header->body_length = length;
	header->message_type = message_type;
	int status = send(socket, header, sizeof(ContentHeader), MSG_WAITALL | MSG_NOSIGNAL);

	if(status < 0)
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_SendHeader - Error al enviar header, codigo: %d", status);

	free(header);

	return status;
}

void* SocketCommons_ReceiveData(int socket, int* message_type, int* error_status)
{
	if(message_type == NULL)
	{
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveData - se recibio NULL en el parametro int* message_type. Se esperaba una direccion de memoria valida.");
		return NULL;
	}

	ContentHeader* header = SocketCommons_ReceiveHeader(socket, error_status);

	if(header == 0)
		return header;

	int len = header->body_length;
	//Informamos al usuario de la funcion que es lo que vamos a recibir para que despues puedan castear nuestro void*
	*message_type = header->message_type;
	free(header);

	Logger_Log(LOG_DEBUG, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveData - Recibiendo datos, length: %d, content type: %d", len, *message_type);

	void* buffer = malloc(len);

	int status = recv(socket, buffer, len, MSG_WAITALL);

	if(status < 1)
	{
		if(error_status != NULL)
		{
			*error_status = errno;
			Logger_Log(LOG_DEBUG, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveData - Return recv: %d, errno: %d, error: %s", status, *error_status, strerror(*error_status));
		}

		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_ReceiveData - Error al recibir datos!");
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

	status = send(socket, data, dataLength, MSG_WAITALL | MSG_NOSIGNAL);

	if(status < 0)
		Logger_Log(LOG_ERROR, "KEMMENSLIB::SOCKETCOMMONS->SocketCommons_SendData - Error al enviar data, codigo: %d", status);

	return status;
}

void cerrarSocket(int descriptorSocket){
      if((close(descriptorSocket) !=0 ))
     {
    	 Logger_Log(LOG_ERROR, "No se pudo cerrar el socket descripto por %d", descriptorSocket);
    	 return;
     }
     Logger_Log(LOG_ERROR, "Socket descripto por %d cerrado, fin de la conexion a traves de el", descriptorSocket);
     return;
 }







