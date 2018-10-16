#ifndef PLANIFICADORCOMMON_H
#define PLANIFICADORCOMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/log.h>

#define LOGFILE "./Planificador.log"

t_log * logger;


void exit_gracefully(int);
void InitPlanificador();
void FreeStringArray(char**);

#endif
