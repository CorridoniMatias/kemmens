#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include "Common.h"
#include <commons/string.h>
#include <string.h>

#define CONFIGFILE "./esiconfig.txt"

typedef struct
{
	char IPPlanificador[15];
	int PuertoPlanificador;
	char IPCoordinador[15];
	int PuertoCoordinador;
} ConfigData;

int ReadConfig();

ConfigData ESIConfig;

void OutputConfigFile();
void CargarConfigEnMemoria();

#endif
