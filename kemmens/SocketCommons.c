#include "kemmens/SocketCommons.h"

ContentHeader* SocketCommons_CreateHeader()
{
	return (ContentHeader*)malloc(sizeof(ContentHeader));
}

int SocketCommons_SendMessageString(int socket, char* message)
{
	return send(socket, message, string_length(message), 0);
}

int SocketCommons_ReceiveHeader(int socket)
{
	//Creamos el header que nos va a dar el lengh del tamaño de lo que sea que vamos a recibir posteriormente en el "body"
	ContentHeader * header = SocketCommons_CreateHeader();
	//Recibimos el Header, al ser un struct compartido, tanto el server como el client lo conocen y saben crearlo para informarle al otro el tamaño del mensaje que van a mandar.
	recv(socket, header, sizeof(ContentHeader), 0);


}
