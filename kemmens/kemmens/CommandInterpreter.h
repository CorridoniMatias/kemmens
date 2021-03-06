#ifndef KEMMENS_COMMANDINTERPRETER_H_
#define KEMMENS_COMMANDINTERPRETER_H_

#include <stdio.h>
#include "logger.h"
#include <string.h>
#include "commons/string.h"
#include "commons/collections/list.h"

//Para deshabilitar el logueo interno de este modulo de las kemmens descomentar la siguiente linea:
//#define COMMANDINTERPRETER_DISABLE_LOGGING

/**
 * 		Estructura que almacena la informacion del interpretador de comandos y su ejecucion
 * 		PARAMETROS:
 * 			command: Comando a analizar y contrastar con la primer palabra de la linea entera
 * 			runner: Funcion a ejecutar al llamar al comando registrado
 * 				PARAMETROS:
 * 					argC: Cantidad de argumentos del comando, sin contar el nombre del mismo
 * 					args: Array de cadenas que almacena los argumentos; el 0 es el comando en si
 * 					callingLine: Copia de la linea entera de comando
 * 					extraData: Informacion extra, opcional
 */
struct
{
	char* command;
	void (*runner)(int argC, char** args, char* callingLine, void* extraData);
} typedef CommandRunnerStructure;

/**
 * 		Estructura donde guardar la informacion para hacer un do threadeado (a traves de un hilo)
 * 		CAMPOS:
 * 			commandline: Linea de comando entera a interpretar
 * 			separator: Caracter que actua de separador para analizar el comando
 * 			data: Datos que la funcion a ejecutar en el hilo tomara como parametros
 * 			postDo: Funcion que se ejecutara en el hilo levantado
 * 				PARAMETROS:
 * 					cmd: Linea de comando entera a interpretar
 * 					sep: Caracter que actua de separador para analizar el comando
 * 					args: Parametros a pasarle a la funcion del hilo
 * 					actionFired: Flag que indica si se dispara una accion o no al interpretar
 */
struct
{
	char* commandline;
	char* separator;
	void* data;
	//free_data indica como se deberia hacer un free de la memoria en void* data
	void (*free_data)(void* data);
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

/*
 * 		Funcion para liberar la estructura de ThreadableDoStructure correctamente.
 * 		Observar el funcionamiento de la funcion para ver como hace las cosas!
 */
void CommandInterpreter_FreeThreadableDoStructure(void* threadableDoStructure);


#endif /* KEMMENS_COMMANDINTERPRETER_H_ */
