#include "kemmens/logger.h"

/*
 * 		Puntero al logger general de las Kemmens, para inicializar usar Logger_CreateLog
 */
t_log* kemmens_global_log = NULL;

t_log* Logger_GetLog()
{
	return kemmens_global_log;
}

void Logger_CreateLog(char* logFile, char* logAlias, bool logToConsole)
{
	if(Logger_IsLoggerValid())
		return;

	kemmens_global_log = log_create(logFile, logAlias, true, LOG_LEVEL_DEBUG);
}

void Logger_DestroyLog()
{
	if(Logger_IsLoggerValid()) //Sin este check y si no se creo el logger -> segmentation fault.
		log_destroy(kemmens_global_log);
}

bool Logger_IsLoggerValid()
{
	return kemmens_global_log != NULL;
}

void Logger_Log(void (*logFunction)(t_log*, const char*), char* message, ...)
{
	if(!Logger_IsLoggerValid())
		return;

	va_list arguments;
	va_start(arguments, message);

	//Al ejecutar snprintf con NULL como buffer nos devuelve la cantidad de bytes que necesitamos malloc-ear
	int strlength = (size_t)vsnprintf(NULL, 0, message, arguments) + 1; //se queda corto por 1 char debe ser por el string end.

	char* formatted = (char*)malloc(strlength);
	vsnprintf(formatted, strlength, message, arguments);

	va_end(arguments);

	logFunction(Logger_GetLog(), formatted);

	free(formatted);
}

