#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include "Common.h"

#include <stdio.h>

#include <commons/config.h>
#include <commons/string.h>

#include <string.h>

typedef struct
{
	char* Algorithm;
	char* IPCoordinador;
	int PortCoordinador;
	int ListenPort;
	int InitEstimation;
	char** BlockedKeys;
} PlanificadorConfig;

PlanificadorConfig ReadConfig();
void FreeConfig(PlanificadorConfig*);
void PrintfConfig(PlanificadorConfig);

PlanificadorConfig ConfigGeneral;

#endif
