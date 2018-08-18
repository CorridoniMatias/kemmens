#include "kemmens/config.h"

///FUNCIONES DE ARCHIVOS DE CONFIGURACION

//Crea un archivo de configuracion a guardar en path, con los campos ingresados por parametro
t_config* archivoConfigCrear(char* path, char** campos) {

	t_config* archivoConfig = config_create(path);
	if(archivoConfigInvalido(archivoConfig, campos))
	{
		puts("Archivo de configuracion invalido\n");
		exit(EXIT_FAILURE);
	}
	return archivoConfig;

}
//---------------------------------------------

//Devuelve true si el archivo apuntado por "archivoConfig" posee el campo "campo", false en caso contrario
bool archivoConfigTieneCampo(t_config* archivoConfig, char* campo) {

	return config_has_property(archivoConfig, campo);

}
//---------------------------------------------

//Contrario a archivoConfigTieneCampo, se fija si el archivo carece del campo
bool archivoConfigFaltaCampo(t_config* archivoConfig, char* campo) {

	return !archivoConfigTieneCampo(archivoConfig, campo);

}
//---------------------------------------------

//Devuelve el valor cadena del campo "campo" en el archivo apuntado por "archivoConfig"
char* archivoConfigSacarStringDe(t_config* archivoConfig, char* campo) {

	return config_get_string_value(archivoConfig, campo);

}
//---------------------------------------------

//Devuelve el valor entero del campo "campo" en el archivo apuntado por "archivoConfig"
int archivoConfigSacarIntDe(t_config* archivoConfig, char* campo) {

	return config_get_int_value(archivoConfig, campo);

}
//---------------------------------------------

//Devuelve el valor long del campo "campo" en el archivo apuntado por "archivoConfig"
long archivoConfigSacarLongDe(t_config* archivoConfig, char* campo) {

	return config_get_long_value(archivoConfig, campo);

}
//---------------------------------------------

//Devuelve el valor doble precision del campo "campo" en el archivo apuntado por "archivoConfig"
double archivoConfigSacarDoubleDe(t_config* archivoConfig, char* campo) {

	return config_get_double_value(archivoConfig, campo);

}
//---------------------------------------------

//Devuelve el array de cadenas del campo "campo" en el archivo apuntado por "archivoConfig"
char** archivoConfigSacarArrayDe(t_config* archivoConfig, char* campo) {

	return config_get_array_value(archivoConfig, campo);

}
//---------------------------------------------

//Devuelve la cantidad de campos que posee el archivo apuntado por "archivoConfig"
int archivoConfigCantidadCampos(t_config* archivoConfig) {

	return config_keys_amount(archivoConfig);

}
//---------------------------------------------

//Destruye el archivo apuntado por "archivoConfig"
void archivoConfigDestruir(t_config* archivoConfig) {

	config_destroy(archivoConfig);

}
//---------------------------------------------

//Guarda el valor "valor" en el campo "campo" del archivo apuntado por "archivoConfig"
void archivoConfigSetearCampo(t_config* archivoConfig, char* campo, char* valor) {

	config_set_value(archivoConfig, campo, valor);

}
//---------------------------------------------

//Devuelve true si el archivo apuntado por "archivoConfig" no existe o bien no posee los campos del array "campos"
bool archivoConfigInvalido(t_config* archivoConfig, char** campos) {

	return (archivoConfigIncompleto(archivoConfig, campos) || archivoConfigInexistente(archivoConfig)) ;
}
//---------------------------------------------

//Devuelve true si el archivo apuntado por "archivoConfig" no existe
bool archivoConfigInexistente(t_config* archivoConfig) {

	return archivoConfig == NULL;

}
//---------------------------------------------

//Devuelve true si no existe un archivo apuntado por "archivoConfig" que posea todos los campos del array "campos"
bool archivoConfigIncompleto(t_config* archivoConfig, char** campos) {

	int indice;
	for(indice = 0; indice < archivoConfigCantidadCampos(archivoConfig); indice++)
	{
		if(archivoConfigFaltaCampo(archivoConfig, campos[indice]))
		{
			return true;
		}
	}
	return false;
}
//---------------------------------------------
