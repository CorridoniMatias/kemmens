#include "kemmens/CommandInterpreter.h"

t_list* interpreters = NULL;

static CommandRunnerStructure* CreateStructure()
{
	return (CommandRunnerStructure*)malloc(sizeof(CommandRunnerStructure));
}

ThreadableDoStructure* CommandInterpreter_MallocThreadableStructure()
{
	ThreadableDoStructure* st = (ThreadableDoStructure*)malloc(sizeof(ThreadableDoStructure));

	st->commandline = NULL;
	st->data = NULL;
	st->separator = NULL;
	st->postDo = NULL;
	st->free_data = NULL;

	return st;
}

void CommandInterpreter_RegisterCommand(char* command, void (*runner)(int argC, char** args,char* callingLine, void* extraData))
{
	if(interpreters == NULL)
	{
		#ifdef KEMMENS_ENABLE_LOGGING
		#ifdef CI_ENABLE_LOGGING
		Logger_Log(LOG_ERROR, "KEMMENSLIB - CommandInterpreter::CommandInterpreter_Do -> No se llamo al metodo CommandInterpreter_Init(...)!!");
		#endif
		#endif
		return;
	}

	CommandRunnerStructure* inter = CreateStructure();

	int len = string_length(command) + 1;
	inter->command = (char*)malloc(len);
	memcpy(inter->command, command, len);

	inter->runner = runner;
	list_add(interpreters, inter);
}

void CommandInterpreter_Init()
{
	if(interpreters == 0)
		interpreters = list_create();
}

bool CommandInterpreter_DeRegisterCommand(char* command)
{
	if(interpreters == NULL)
	{
		#ifdef KEMMENS_ENABLE_LOGGING
		#ifdef CI_ENABLE_LOGGING
		Logger_Log(LOG_ERROR, "KEMMENSLIB - CommandInterpreter::CommandInterpreter_Do -> No se llamo al metodo CommandInterpreter_Init(...)!!");
		#endif
		#endif
		return false;
	}

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

void* CommandInterpreter_DoThreaded(void* arg) //Thread-intended function
{
	ThreadableDoStructure* args = (ThreadableDoStructure*)arg;

	bool res = CommandInterpreter_Do(
				args->commandline,
				args->separator,
				args->data
			);

	if(args->postDo != NULL)
		args->postDo(
				args->commandline,
				args->separator,
				args->data,
				res
			);

	free(args);
	return 0;
}

bool CommandInterpreter_Do(char* command, char* separator, void* extraData)
{
	if(interpreters == NULL)
	{
		#ifdef KEMMENS_ENABLE_LOGGING
		#ifdef CI_ENABLE_LOGGING
		Logger_Log(LOG_ERROR, "KEMMENSLIB - CommandInterpreter::CommandInterpreter_Do -> No se llamo al metodo CommandInterpreter_Init(...)!!");
		#endif
		#endif
		return false;
	}

	if(string_length(command) == 0)
		return false;

	CommandRunnerStructure* inter;
	int size = list_size(interpreters);

	char** cmd = string_split(command, separator);

	int argCo = 0;

	while (cmd[argCo] != NULL) {
		argCo++;
	}

	if(argCo > 0) // si tenemos al menos el comando
		for(int i = 0; i < size; i++)
		{
			inter = (CommandRunnerStructure*)list_get(interpreters, i);

			if(strcmp(cmd[0], inter->command) == 0)
			{
				if(inter->runner != NULL)
					inter->runner(--argCo, cmd, command, extraData);//Sacamos 1 porque el primer arg es el comando en si y esta variable va a decir cuantos parametros hay.

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

void CommandInterpreter_Destroy()
{
	if(interpreters == NULL)
		return;

	void deregistercommands(void* cmd)
	{
		CommandRunnerStructure* com = (CommandRunnerStructure*)cmd;
		#ifdef KEMMENS_ENABLE_LOGGING
		#ifdef CI_ENABLE_LOGGING
		Logger_Log(LOG_INFO, "KEMMENSLIB - CommandInterpreter::CommandInterpreter_Destroy -> Eliminando comando '%s'", com->command);
		#endif
		#endif
		free(com->command);
	}

	list_iterate(interpreters, deregistercommands);
	list_destroy_and_destroy_elements(interpreters, (void*)free);
}

void CommandInterpreter_FreeThreadableDoStructure(void* threadableDoStructure)
{
	ThreadableDoStructure* tds = (ThreadableDoStructure*)threadableDoStructure;

	free(tds->commandline);

	if(tds->free_data != NULL)
		tds->free_data(tds->data);

	free(tds);
}

