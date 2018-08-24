#include "commons/log.h"
#include "readline/readline.h"
#include "kemmens/megekemmen.h"
#include "kemmens/SocketServer.h"
#include "kemmens/SocketClient.h"
#include "kemmens/ThreadManager.h"
#include "kemmens/logger.h"
#include "kemmens/SocketMessageTypes.h"
#include "kemmens/SocketCommons.h"
#include "kemmens/CommandInterpreter.h"
#include <unistd.h>

bool recibir = true;

void cleanup()
{
	//Aca se podria hacer un cleanup custom
}

void onPacketArrived(int socketID, int message_type, void* data)
{
	if(message_type == MESSAGETYPE_STRING)
	{
		printf("STRING RECIBIDO: %s\n", ((char*)data) );
		SocketCommons_SendMessageString(socketID, "Recibido!");
	}

	free(data);
}

void exitok()
{
	exit_gracefully_custom(cleanup, EXIT_SUCCESS);
}

void processLineInput(char* line)
{
	printf("Comando: %s\n", line);
	if(strcmp(line, "stop") == 0)
	{
		free(line);
		SocketServer_Stop();
		return;
	}

	free(line);
}

void* Server()
{
	SocketServer_Start("CHAT", 8086);
	SocketServer_ListenForConnection(onPacketArrived, processLineInput);
	printf("SERVER SHUTDOWN\n");
	//SocketServer_ListenForConnection(logger, 0);
	return NULL;
}

void* ClientManageReceptions(void* socket)
{
	char* res;
	//while(1)
	{
		res = SocketCommons_ReceiveString( ((int)socket) );
		if(res != 0)
			printf("Response received '%s'!\n", res);
		else
			recibir = false;
	}

	return 0;
}

void Client(char* texto)
{
	printf("Conectando al server...\n");
	int sock = SocketClient_ConnectToServer("8086");
	printf("Socket asignado %d\n", sock);
	int m;
	pthread_t threadRecv;
	while(1)
	{
		if(!recibir)
			break;

		printf("WHILE\n");
		m = SocketCommons_SendMessageString(sock, texto);
		printf("String enviado. Retorno %d\n", m);

		ThreadManager_CreateThread(&threadRecv, ClientManageReceptions, ((void*) sock) );
		printf("Awaiting response...\n");
		pthread_join(threadRecv, NULL);

		sleep(1);
	}
}

void* ClientServer(void* port)
{
	printf("Conectando al server...\n");
	int sock = SocketClient_ConnectToServer( ((char*)port) );
	printf("Socket asignado %d\n", sock);
	int m;
	char* asd = (char*)malloc(1);
	while(1)
	{
		printf("WHILE\n");
		m = SocketCommons_SendMessageString(sock, "Hello World!");
		printf("String enviado. Retorno %d\n", m);
		int st = recv(sock, asd, 0, MSG_WAITALL);
		free(asd);
		printf("RECV = %d\n", st);
		if(st == 0)
			break;
		sleep(1);
	}

	return NULL;
}

int main(int argc, char **argv)
{

	char** cmd = string_split("print hello world", " ");

	int argCo = 0;
	int i = 0;
	while (cmd[i] != NULL) {
		argCo++;
		i++;
	}
	argCo--;

	/*if(--argCo > 0)
	{
		free(*cmd);
		cmd++;
	}*/

	for(int o = 1; o <= argCo;o++) {
		printf("Arg %d de %d args: '%s'\n", o,argCo, cmd[o]);
		free(cmd[o]);
	}

	free(cmd);

	printf("Hay %d args\n", argCo);

	return 0;
	Logger_CreateLog("./chatapp.log", "CHARAPP", true);

	if(argc < 2)
	{
		printf("Debe ingresar el nombre de la instancia como parametro.\n");
		exitok();
	}

	Logger_Log(LOG_INFO, "Iniciando ChatApp con instance name %s...", argv[1]);

	if(strcmp("cli", argv[1]) == 0)
	{
		if(argc < 3)
		{
			printf("Debe ingresar el texto a mandar como parametro.\n");
			exitok();
		}

		Client(argv[2]);
		printf("Client end\n");
	}
	else
	{
		if(argc < 3)
		{
			Server();
		} else
		{
			pthread_t threadServer, threadClient;

			ThreadManager_CreateThread(&threadServer, Server, NULL);
			ThreadManager_CreateThread(&threadClient, ClientServer, ((void*) argv[2]) );
			printf("RUNNING THREADS\n");
			pthread_join(threadServer, NULL);
			pthread_join(threadClient, NULL);
			printf("JOINED BOTH\n");
		}
	}

	exitok();
}
