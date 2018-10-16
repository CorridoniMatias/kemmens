#include "ConfigReader.h"

void OutputConfigFile()
{
	int status = ReadConfig();
	printf("Status: %d\n", status);
	printf("%s\n", ESIConfig.IPCoordinador);
	printf("%s\n", ESIConfig.IPPlanificador);
	printf("%d\n",ESIConfig.PuertoCoordinador);
	printf("%d\n",ESIConfig.PuertoPlanificador);
}

void CargarConfigEnMemoria()
{
	if(ReadConfig() == -1)
		exit_gracefully(EXIT_FAILURE);
}

int ReadConfig()
{
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	char** comando = NULL;

	ESIConfig.PuertoCoordinador = -1;
	ESIConfig.PuertoPlanificador = -1;

	fp = fopen(CONFIGFILE, "r");

	if (fp == NULL){
		log_error(logger, "Error al abrir el archivo de configuracion para ESIProcess.");
		return -1;
	}

	while (getline(&line, &len, fp) != -1) {
		comando = string_split(line, "=");

		if(comando[0] != NULL && comando[1] != NULL)
		{
			if(strcmp(comando[0], "IPPlanificador") == 0)
			{
				string_trim(&comando[1]);
				strcpy(ESIConfig.IPPlanificador, comando[1]);
			}
			else if(strcmp(comando[0], "IPCoordinador") == 0)
			{
				string_trim(&comando[1]);
				strcpy(ESIConfig.IPCoordinador, comando[1]);
			}
			else if(strcmp(comando[0], "PuertoPlanificador") == 0)
				ESIConfig.PuertoPlanificador = atoi(comando[1]);
			else if(strcmp(comando[0], "PuertoCoordinador") == 0)
				ESIConfig.PuertoCoordinador = atoi(comando[1]);
		}
	}

	fclose(fp);
	if (line)
		free(line);

	free(comando);

	if((ESIConfig.IPCoordinador == NULL || string_length(ESIConfig.IPCoordinador) == 0)	||
		(ESIConfig.IPPlanificador == NULL || string_length(ESIConfig.IPPlanificador) == 0) 	||
	ESIConfig.PuertoCoordinador == -1	||
	ESIConfig.PuertoPlanificador == -1)
	{
		log_error(logger, "Faltan parametros para la configuracion.");
		return -1;
	}

	return 1;
}
