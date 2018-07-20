#include "megekemmen.h"

void exit_gracefully(int exit_code)
{
	exit(exit_code);
}

void exit_gracefully_custom(void (*operations)(),int exit_code)
{
	operations();
	exit(exit_code);
}

