#ifndef KEMMENS_COMMANDINTERPRETER_H_
#define KEMMENS_COMMANDINTERPRETER_H_

#include <stdio.h>
#include "logger.h"
#include <string.h>
#include "commons/string.h"
#include "commons/collections/list.h"

struct
{
	char* command;
	void (*runner)(int argC, char** args, char* callingLine, void* extraData);
} typedef CommandRunnerStructure;

struct
{
	char* commandline;
	char* separator;
	void* data;
	void (*postDo)(char* cmd, char* sep, void* args, bool actionFired);
} typedef ThreadableDoStructure;

/*
 *		Registra un comando para que se llame a runner cuando llegue el comando registrado a traves de CommandInterpreter_Do(...).
 *
 *		runner = Funcion que se va a ejecutar si el comando registrado es llamado.
 *
 *			argC = Cantidad de argumentos del comando. Ej: print hello world -> los args. son hello y world entonces tiene 2 argumentos
 *
 *			args = Argumentos, el indice 0 es el comando, los demas son los argumentos pasados.
 *
 *			callingLine = Copia de la linea de comando.
 *
 *			extraData = Cualquier data extra.
 *
 *			SIEMPRE al final de la funcion runner desarrollada llamar a CommandInterpreter_FreeArguments(args)!!!!
 *
 *
 */
void CommandInterpreter_RegisterCommand(char* command, void (*runner)(int argC, char** args, char* callingLine, void* extraData));

/*
 *		Inicializa el interpretador. Debe llamarse esta funcion antes de cualquier otro.
 */
void CommandInterpreter_Init();

/*
 *		Ejecuta la accion de ver si algun comando registrado coincide con el comando que se pasa como parametro.
 *
 *		command = linea de comando entera.
 *
 *		separator = string por el cual se va a hacer el split de command para analizarlo.
 *
 *		extraData = cualquier dato que se le quiera enviar la funcion del comando a ejecutar.
 *
 *		Devuelve TRUE si el comando fue encontrado y se ejecuto la funcion vinculada, FALSE si no se encontro ningun comando para la linea en 'command'.
 */
bool CommandInterpreter_Do(char* command, char* separator, void* extraData);

/*
 *		Util para los callbacks de los comandos, hace un free de un char**.
 */
void CommandInterpreter_FreeArguments(char** args);

/*
 *		Deregistra todos los comandos y libera la memoria reservada.
 */
void CommandInterpreter_Destroy();

/*
 * 		Hace lo mismo que el DO pero se puede threadear, se usa ThreadableDoStructure para los argumentos.
 */
void* CommandInterpreter_DoThreaded(void* arg);

/*
 *		Crea estructura del interpretador para thredear el Do.
 */
ThreadableDoStructure* CommandInterpreter_MallocThreadableStructure();


#endif /* KEMMENS_COMMANDINTERPRETER_H_ */
