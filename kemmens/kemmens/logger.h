#ifndef KEMMENS_LOGGER_H_
#define KEMMENS_LOGGER_H_

#include "commons/log.h"
#include <stdlib.h>
#include <stdarg.h>

#define LOG_ERROR (void*)log_error
#define LOG_INFO (void*)log_info
#define LOG_DEBUG (void*)log_debug

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

/*
 * 		Hace un logueo, logFunction es la funcion que se va a usar para hacer el logueo propiamente dicho (log_info, log_error, ...)
 *
 * 		Este wrapper brinda seguridad para no tener problemas de segmentation fault en caso que el log sea nulo y se intente loguear algo.
 *
 * 		Ejemplo: Logger_Log((void*)log_debug, "Hola! %d", 1);
 */
void Logger_Log(void (*logFunction)(t_log*, const char*), char* message, ...);

#endif /* KEMMENS_LOGGER_H_ */
