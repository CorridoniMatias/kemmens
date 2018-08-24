#include "kemmens/megekemmen.h"
#include "kemmens/logger.h"
#include "kemmens/CommandInterpreter.h"

void exit_gracefully(int exit_code)
{
	CommandInterpreter_Destroy();
	Logger_DestroyLog();	//Siempre destruir el logger al final en lo posible porque puede ser que otras cosas todavia lo esten usando.
	exit(exit_code);
}

void exit_gracefully_custom(void (*operations)(),int exit_code)
{
	operations();
	exit_gracefully(exit_code);
}

