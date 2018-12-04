#include "kemmens/SocketClient.h"

int SocketClient_ConnectToServerIP(char* ip, char* puerto)
{

	struct addrinfo auxiliar;		//Temporal, para guardar transitoriamente datos de la direccion a la cual me quiero conectar
	struct addrinfo* infoServidor;	//La posta, ahi hare el getaddrinfo

	memset(&auxiliar, 0, sizeof(auxiliar));
	auxiliar.ai_family = AF_UNSPEC;    		//El sistema verifica que familia de direcciones usaremos (IPv4 o IPv6)
	auxiliar.ai_socktype = SOCK_STREAM;  	//Protocolo a usar es TCP/IP

	getaddrinfo(ip, puerto, &auxiliar, &infoServidor);	//Guardo info de la direccion aca

	int socketServidor;						//Descriptor del socket con la direccion del server
	//Preparamos el socket con el tipo de conexion deseado
	socketServidor = socket(infoServidor->ai_family, infoServidor->ai_socktype, infoServidor->ai_protocol);

	if(socketServidor == -1)				//Si al querer armarlo devolvio < 0, algo salio mal
		return -1;

	// Ahora si, queremos conectarnos al servidor a traves de ese socket!
	int retorno = connect(socketServidor, infoServidor->ai_addr, infoServidor->ai_addrlen);

	freeaddrinfo(infoServidor);				//Es esto o memory leak

	if(retorno < 0)
	{
		return -1;
	}
	else
	{
		return socketServidor;				//Todo marcha bien Milhouse!
	}

}
