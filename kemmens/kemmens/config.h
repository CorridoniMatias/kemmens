#ifndef CONFIGKEMMENS_H_
#define CONFIGKEMMENS_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/config.h>
#include "logger.h"
#include "StringUtils.h"

//Para deshabilitar el logueo interno de este modulo de las kemmens descomentar la siguiente linea:
//#define CONFIG_DISABLE_LOGGING

/**
 * 		ACCION: Crea un archivo de configuracion en cierta ruta y con ciertos campos
 * 		PARAMETROS:
 * 			path: Ruta de destino del archivo
 * 			campos: Array de cadenas con los campos a insertar en el archivo
 */
t_config* archivoConfigCrear(char* path, char** campos);

/**
 * 		ACCION: Me dice si un archivo posee cierto campo
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar
 * 			campo: Campo a buscar
 */
bool archivoConfigTieneCampo(t_config* archivoConfig, char* campo);

/**
 * 		ACCION: Devuelve el valor de cierto campo de un archivo, en forma de cadena (char*)
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar
 * 			campo: Campo cuyo valor quiero extraer
 */
char* archivoConfigSacarStringDe(t_config* archivoConfig, char* campo);

/**
 * 		ACCION: Devuelve el valor de cierto campo de un archivo, en forma de int
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar
 * 			campo: Campo cuyo valor quiero extraer
 */
int archivoConfigSacarIntDe(t_config* archivoConfig, char* campo);

/**
 * 		ACCION: Devuelve el valor de cierto campo de un archivo, en forma de long
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar
 * 			campo: Campo cuyo valor quiero extraer
 */
long archivoConfigSacarLongDe(t_config* archivoConfig, char* campo);

/**
 * 		ACCION: Devuelve el valor de cierto campo de un archivo, en forma de double
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar
 * 			campo: Campo cuyo valor quiero extraer
 */
double archivoConfigSacarDoubleDe(t_config* archivoConfig, char* campo);

/**
 * 		ACCION: Devuelve el valor de cierto campo de un archivo, en forma de array de cadenas
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar
 * 			campo: Campo cuyo valor quiero extraer
 */
char** archivoConfigSacarArrayDe(t_config* archivoConfig, char* campo);

/**
 * 		ACCION: Retorna la cantidad de campos que posee un archivo de configuracion
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar
 */
int archivoConfigCantidadCampos(t_config* archivoConfig);

/**
 * 		ACCION: Destruye un archivo de configuracion determinado
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a destruir
 */
void archivoConfigDestruir(t_config* archivoConfig);

/**
 * 		ACCION: Actualiza el valor de un campo de un cierto archivo de configuracion
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a modificar
 * 			campo: Campo cuyo valor quiero modificar
 * 			valor: Valor a asignar a dicho campo
 */
void archivoConfigSetearCampo(t_config* archivoConfig, char* campo, char* valor);

/**
 * 		ACCION: Devuelve true si el archivo apuntado no existe, o bien si no posee los campos especificados
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a analizar/buscar
 * 			campos: Campos a verificar en el archivo
 */
bool archivoConfigInvalido(t_config* archivoConfig, char** campos);

/**
 * 		ACCION: Devuelve true si el archivo apuntado no existe
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a buscar
 */
bool archivoConfigInexistente(t_config* archivoConfig);

/**
 * 		ACCION: Devuelve true si el archivo apuntado no posee los campos especificados
 * 		PARAMETROS:
 * 			archivoConfig: Puntero al archivo a buscar
 * 			campos: Campos a verificar en el archivo
 */
bool archivoConfigIncompleto(t_config* archivoConfig, char** campos);

/**
 * 		ACCION: Informa que el archivo es invalido en logs y termina el programa con error.
 */
void archivoConfigEsInvalido();

#endif /* CONFIGKEMMENS_H_ */
