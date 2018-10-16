#include "Common.h"
#include "SocketServer.h"

int main(int argc, char **argv){
	InitPlanificador();

	StartSocketServer("plan", logger);
	ListenForConnection(logger);

	exit_gracefully(EXIT_SUCCESS);
}

