#ifndef CONFIGKEMMENS_H_
#define CONFIGKEMMENS_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/config.h>


//Funciones de archivos de configuracion
t_config* archivoConfigCrear(char* path, char** campos);
bool archivoConfigTieneCampo(t_config* archivoConfig, char* campo);
bool archivoConfigFaltaCampo(t_config* archivoConfig, char* campo);
char* archivoConfigSacarStringDe(t_config* archivoConfig, char* campo);
int archivoConfigSacarIntDe(t_config* archivoConfig, char* campo);
long archivoConfigSacarLongDe(t_config* archivoConfig, char* campo);
double archivoConfigSacarDoubleDe(t_config* archivoConfig, char* campo);
char** archivoConfigSacarArrayDe(t_config* archivoConfig, char* campo);
int archivoConfigCantidadCampos(t_config* archivoConfig);
void archivoConfigDestruir(t_config* archivoConfig);
void archivoConfigSetearCampo(t_config* archivoConfig, char* campo, char* valor);
bool archivoConfigInvalido(t_config* archivoConfig, char** campos);
bool archivoConfigInexistente(t_config* archivoConfig);
bool archivoConfigIncompleto(t_config* archivoConfig, char** campos);
void archivoConfigEsInvalido();

#endif /* CONFIGKEMMENS_H_ */
