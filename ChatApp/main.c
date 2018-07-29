#include "commons/log.h"
#include "readline/readline.h"
#include "kemmens/megekemmen.h"
#include "kemmens/SocketServer.h"
#include "kemmens/SocketClient.h"
#include "kemmens/ThreadManager.h"
#include <unistd.h>

t_log * logger;

void cleanup()
{
	if(logger != NULL) //Sin este check y si no se creo el logger -> segmentation fault.
		log_destroy(logger);
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
	SocketServer_Start("CHAT", logger, 8086);
	SocketServer_ListenForConnection(logger, processLineInput);
	printf("SERVER SHUTDOWN\n");
	//SocketServer_ListenForConnection(logger, 0);
	return NULL;
}

void Client(char* texto)
{
	printf("Conectando al server...\n");
	int sock = SocketClient_ConnectToServer("8086");
	printf("Socket asignado %d\n", sock);
	int m;
	//char* asd = (char*)malloc(1);
	while(1)
	{
		printf("WHILE\n");
		m = SocketCommons_SendMessageString(sock, texto);
		printf("String enviado. Retorno %d\n", m);
		//int st = recv(sock, asd, 0, MSG_WAITALL);
		//free(asd);
		//printf("RECV = %d\n", st);
		//if(st == 0)
			//break;
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
	logger = log_create("./chatapp.log", "CHATAPP", true, LOG_LEVEL_DEBUG);
	if(argc < 2)
	{
		printf("Debe ingresar el nombre de la instancia como parametro.\n");
		exitok();
	}

	log_info(logger, "Iniciando ChatApp con instance name %s...", argv[1]);

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

			ThreadManager_CreateThread(logger, &threadServer, Server, NULL);
			ThreadManager_CreateThread(logger, &threadClient, ClientServer, ((void*) argv[2]) );
			printf("RUNNING THREADS\n");
			pthread_join(threadServer, NULL);
			pthread_join(threadClient, NULL);
			printf("JOINED BOTH\n");
		}
	}

	exitok();
}
