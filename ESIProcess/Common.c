#include "Common.h"

void InitEsiProcess()
{
	logger = log_create(LOGFILE, "ESIProcess", true, LOG_LEVEL_TRACE);
}

void FreeStringArray(char** array)
{
	int i = 0;
	while(array[i] != NULL)
	{
		free(array[i]);
		i++;
	}

	free(array);
}

//Todo lo que se debe liberar al hacer un exit se debe agregar en esta funcion en caso que no haya sido liberado antes!

void exit_gracefully(int exit_code) {
	log_destroy(logger);
	exit(exit_code);
}
