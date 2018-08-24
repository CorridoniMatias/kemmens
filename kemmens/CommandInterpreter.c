#include "kemmens/CommandInterpreter.h"

static CommandRunnerStructure* CreateStructure()
{
	return (CommandRunnerStructure*)malloc(sizeof(CommandRunnerStructure));
}

void CommandInterpreter_RegisterCommand(char* command, void (*runner)(int argC, char** args, void* extraData))
{
	CommandRunnerStructure* inter = CreateStructure();

	int len = string_length(command) + 1;
	inter->command = (char*)malloc(len);
	memcpy(inter->command, command, len);

	inter->runner = runner;
}

void CommandInterpreter_InitInterpreter()
{
	interpreters = list_create();
}

bool CommandInterpreter_DeRegisterCommand(char* command)
{
	CommandRunnerStructure* inter;
	int size = list_size(interpreters);
	for(int i = 0; i < size; i++)
	{
		inter = (CommandRunnerStructure*)list_get(interpreters, i);

		if(strcmp(command, inter->command) == 0)
		{
			free(inter->command);
			list_remove(interpreters, i);
			free(inter);

			return true;
		}
	}

	return false;
}

bool CommandInterpreter_Do(char* command, char* separator, void* extraData)
{
	CommandRunnerStructure* inter;
	int size = list_size(interpreters);

	char** cmd = string_split(command, separator);

	int argCo = 0;

	while (cmd[argCo] != NULL) {
		argCo++;
	}

	argCo--; //Sacamos 1 porque el primer arg es el comando en si y esta variable va a decir cuantos parametros hay.

	for(int i = 0; i < size; i++)
	{
		inter = (CommandRunnerStructure*)list_get(interpreters, i);

		if(strcmp(command, inter->command) == 0)
		{
			inter->runner(argCo, cmd, extraData);

			return true;
		}
	}

	CommandInterpreter_FreeArguments(cmd); //Si ninguna de las funciones registradas va a usar el comando tenemos que liberar nosotros el recurso.

	return false;
}

void CommandInterpreter_FreeArguments(char** args)
{
	int i = 0;
	while (args[i] != NULL) {
		free(args[i]);
		i++;
	}
	free(args);
}

