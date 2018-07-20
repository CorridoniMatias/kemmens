#include "Common.h"
#include "ConfigHandler.h"

void InitPlanificador()
{
	logger = log_create(LOGFILE, "Planificador", true, LOG_LEVEL_TRACE);
	ConfigGeneral = ReadConfig();
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

void exit_gracefully_plan(int exit_code) {
	FreeConfig(&ConfigGeneral);
	log_destroy(logger);
}
