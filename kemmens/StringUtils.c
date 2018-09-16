#include "kemmens/StringUtils.h"


char* StringUtils_Format(char* plain, ...)
{
	va_list arguments;
	va_start(arguments, plain);

	//Al ejecutar snprintf con NULL como buffer nos devuelve la cantidad de bytes que necesitamos malloc-ear
	int strlength = (size_t)vsnprintf((void*)0, 0, plain, arguments) + 1; //se queda corto por 1 char debe ser por el string end.

	char* formatted = (char*)malloc(strlength);
	vsnprintf(formatted, strlength, plain, arguments);

	va_end(arguments);

	return formatted;
}

