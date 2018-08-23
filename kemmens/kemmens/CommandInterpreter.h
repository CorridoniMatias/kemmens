#ifndef KEMMENS_COMMANDINTERPRETER_H_
#define KEMMENS_COMMANDINTERPRETER_H_

#include <stdio.h>
#include "logger.h"
#include <string.h>
#include "commons/string.h"
#include "commons/collections/list.h"

t_list* interpreters = 0;

struct
{
	char* command;
	void (*runner)(int argC, char** args, void* extraData);
} typedef CommandRunnerStructure;




#endif /* KEMMENS_COMMANDINTERPRETER_H_ */
