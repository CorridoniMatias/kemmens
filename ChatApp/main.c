#include "kemmens/megekemmen.h"
#include "commons/log.h"
#include "readline/readline.h"
#include "kemmens/SocketServer.h"
#include "kemmens/SocketClient.h"

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

void Server()
{
	SocketServer_Start("CHAT", logger, 8080);
	SocketServer_ListenForConnection(logger);
}

void Client()
{
	printf("Conectando al server...\n");
	int sock = SocketClient_ConnectToServer("8080");
	printf("Socket asignado %d\n", sock);
	//SocketCommons_SendMessageString(sock, "hola");
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
		Client();
	else
		Server();

	exitok();
}
