#include "kemmens/config.h"

t_config* archivoConfigCrear(char* path, char** campos) {

	t_config* archivoConfig = config_create(path);
	if (archivoConfigInvalido(archivoConfig, campos)) {
		archivoConfigEsInvalido();
	}
	return archivoConfig;

}

bool archivoConfigTieneCampo(t_config* archivoConfig, char* campo) {

	return config_has_property(archivoConfig, campo);

}

char* archivoConfigSacarStringDe(t_config* archivoConfig, char* campo) {

	return config_get_string_value(archivoConfig, campo);

}

int archivoConfigSacarIntDe(t_config* archivoConfig, char* campo) {

	return config_get_int_value(archivoConfig, campo);

}

long archivoConfigSacarLongDe(t_config* archivoConfig, char* campo) {

	return config_get_long_value(archivoConfig, campo);

}

double archivoConfigSacarDoubleDe(t_config* archivoConfig, char* campo) {

	return config_get_double_value(archivoConfig, campo);

}

char** archivoConfigSacarArrayDe(t_config* archivoConfig, char* campo) {

	return config_get_array_value(archivoConfig, campo);

}

int archivoConfigCantidadCampos(t_config* archivoConfig) {

	return config_keys_amount(archivoConfig);

}

void archivoConfigDestruir(t_config* archivoConfig) {

	config_destroy(archivoConfig);

}

void archivoConfigSetearCampo(t_config* archivoConfig, char* campo, char* valor) {

	config_set_value(archivoConfig, campo, valor);

}

bool archivoConfigInvalido(t_config* archivoConfig, char** campos) {
	return (archivoConfigInexistente(archivoConfig)
			|| archivoConfigIncompleto(archivoConfig, campos));
}

bool archivoConfigInexistente(t_config* archivoConfig) {

	return archivoConfig == NULL;

}

bool archivoConfigIncompleto(t_config* archivoConfig, char** campos) {

	int indice;
	int cantCampos = StringUtils_ArraySize(campos);
	for (indice = 0; indice < cantCampos; indice++) {

		if (!archivoConfigTieneCampo(archivoConfig, campos[indice])) {
			return true;
		}

	}
	return false;
}

void archivoConfigEsInvalido() {
	#ifdef KEMMENS_ENABLE_LOGGING
	#ifdef CONFIG_ENABLE_LOGGING
	Logger_Log(LOG_DEBUG, "KEMMENS::Config -> Archivo de configuracion invalido\n");
	#endif
	#endif
	exit(EXIT_FAILURE);
}
