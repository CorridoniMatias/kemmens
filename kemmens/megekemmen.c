#include "kemmens/megekemmen.h"
#include "kemmens/logger.h"

void exit_gracefully(int exit_code)
{
	Logger_DestroyLog();
	exit(exit_code);
}

void exit_gracefully_custom(void (*operations)(),int exit_code)
{
	operations();
	exit_gracefully(exit_code);
}

