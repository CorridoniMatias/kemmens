#ifndef KEMMENS_LOGGER_H_
#define KEMMENS_LOGGER_H_

#include "commons/log.h"
#include <stdlib.h>
#include <stdarg.h>

/*
 * 		Obtiene el puntero al log.
 */
t_log* Logger_GetLog();

/*
 * 		Asigna el log a la variable de logueo de las Kemmens.
 */
void Logger_CreateLog(char* logFile, char* logAlias, bool logToConsole);

/*
 *		Hace free() del log. Esto se llama automaticamente desde exit_gracefully en megekemmen!
 */
void Logger_DestroyLog();


/*
 * 		Comprueba si el logger inicializado.
 */
bool Logger_IsLoggerValid();

#endif /* KEMMENS_LOGGER_H_ */
