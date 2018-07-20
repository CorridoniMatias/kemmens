#include "Common.h"
#include "SocketServer.h"
#include "ConfigHandler.h"


int main(int argc, char **argv){
	InitPlanificador();

	StartSocketServer("PLAN", logger, ConfigGeneral.ListenPort);
	ListenForConnection(logger);
	exit_gracefully_custom(exit_gracefully_plan, EXIT_SUCCESS);
}

