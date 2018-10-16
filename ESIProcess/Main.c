#include "ConfigReader.h"

int main(int argc, char **argv){

	InitEsiProcess();
	CargarConfigEnMemoria();
	OutputConfigFile();


	exit_gracefully(EXIT_SUCCESS);
}

