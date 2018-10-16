#ifndef ESICOMMON_H
#define ESICOMMON_H

#include <stdlib.h>
#include <commons/log.h>

#define LOGFILE "./ESIProcessLog.log"

t_log * logger;


void exit_gracefully(int);
void InitEsiProcess();
void FreeStringArray(char**);

#endif
