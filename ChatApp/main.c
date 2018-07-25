#include "kemmens/megekemmen.h"
#include "commons/log.h"
#include "readline/readline.h"
#include "kemmens/SocketServer.h"
#include "kemmens/SocketClient.h"
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

void Server()
{
	SocketServer_Start("CHAT", logger, 8085);
	SocketServer_ListenForConnection(logger, processLineInput);
	//SocketServer_ListenForConnection(logger, 0);
}

void Client(char* texto)
{
	printf("Conectando al server...\n");
	int sock = SocketClient_ConnectToServer("8085");
	printf("Socket asignado %d\n", sock);
	int m;
	while(1)
	{
		m = SocketCommons_SendMessageString(sock, texto);
		printf("String enviado. Retorno %d\n", m);
		sleep(1);
	}
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
	}
	else
		Server();

	exitok();
}
