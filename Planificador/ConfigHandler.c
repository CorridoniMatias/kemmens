#include "ConfigHandler.h"

void FreeConfig(PlanificadorConfig* config)
{
	if(config == NULL)
		return;

	free(config->Algorithm);
	free(config->IPCoordinador);
	FreeStringArray(config->BlockedKeys);
}

PlanificadorConfig ReadConfig()
{
	PlanificadorConfig pconfig;

	t_config* config = config_create("./planificador_config.txt");

	pconfig.ListenPort = config_get_int_value(config, "ListenPort");
	pconfig.InitEstimation = config_get_int_value(config, "InitEstimation");
	pconfig.PortCoordinador = config_get_int_value(config, "PortCoordinador");

	char* alg = config_get_string_value(config, "Algorithm");
	char* ipcoord = config_get_string_value(config, "IPCoordinador");

	pconfig.Algorithm = (char*) malloc(string_length(alg) + 1);
	pconfig.IPCoordinador = (char*) malloc(string_length(ipcoord) + 1);

	memcpy(pconfig.Algorithm, alg, string_length(alg) + 1);
	memcpy(pconfig.IPCoordinador, ipcoord, string_length(ipcoord) + 1);

	char* bkeys = config_get_string_value(config, "BlockedKeys");

	pconfig.BlockedKeys = string_get_string_as_array(bkeys);

	config_destroy(config);

	return pconfig;
}

void PrintfConfig(PlanificadorConfig config)
{
	printf("%s\n", config.IPCoordinador);
	printf("%s\n", config.Algorithm);
	printf("%d\n",config.InitEstimation);
	printf("%d\n",config.PortCoordinador);
	printf("%d\n",config.ListenPort);

	int i = 0;
	while(config.BlockedKeys[i] != NULL)
	{
		printf("%s\n", config.BlockedKeys[i]);
		i++;
	}
}
