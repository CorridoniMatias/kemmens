#include "commons/log.h"
#include "readline/readline.h"
#include "kemmens/megekemmen.h"
#include "kemmens/SocketServer.h"
#include "kemmens/SocketClient.h"
#include "kemmens/ThreadManager.h"
#include "kemmens/logger.h"
#include "kemmens/SocketMessageTypes.h"
#include "kemmens/SocketCommons.h"
#include "kemmens/CommandInterpreter.h"
#include "kemmens/ThreadPool.h"
#include "kemmens/Serialization.h"
#include "kemmens/StringUtils.h"
#include "kemmens/Utils.h"
#include <unistd.h>

bool recibir = true;
ThreadPool* pool;

void cleanup()
{
	//Aca se podria hacer un cleanup custom
}

void exitok()
{
	exit_gracefully_custom(cleanup, EXIT_SUCCESS);
}

void* postDo(char* cmd, char* sep, void* args, bool fired)
{
	if(!fired)
		SocketCommons_SendMessageString((int)args, "Lo recibido no es comando!");

	free(cmd);
 return 0;
}

void onPacketArrived(int socketID, int message_type, void* data, int message_length)
{
	printf("--->s %d bytes recibidos\n", message_length );
	if(message_type == MESSAGETYPE_STRING)
	{
		printf("STRING RECIBIDO: %s\n", ((char*)data) );

		ThreadableDoStructure* st = CommandInterpreter_MallocThreadableStructure();

		st->commandline = (char*)data;
		st->data = (void*)socketID;
		st->separator = " ";
		st->postDo = (void*)postDo;

		ThreadPoolRunnable* run = ThreadPool_CreateRunnable();

		run->data = (void*)st;
		run->runnable = (void*)CommandInterpreter_DoThreaded;
		run->free_data = (void*)CommandInterpreter_FreeThreadableDoStructure;

		ThreadPool_AddJob(pool, run);

		//CommandInterpreter_Do((char*)data, " ", (void*)socketID);
	}

	//free(data);
}

void processLineInput(char* line)
{
	printf("Comando: %s\n", line);

	CommandInterpreter_Do(line, " ", NULL);

	free(line);
}

void *CommandDouble (int argC, char** args, char* callingLine, void* extraData)
{
	if(argC == 1)
	{
		char* format = (char*)malloc(2);
		int intval = atoi(args[1]);
		if(intval == 2)
			sleep(10);

		snprintf(format, 2, "%d", intval*2);

		if(SocketServer_IsClientConnected((int)extraData))
			SocketCommons_SendMessageString((int)extraData, format);
		free(format);
	}

	CommandInterpreter_FreeArguments(args);
	return 0;
}

void *CommandPrivate (int argC, char** args, char* callingLine, void* extraData)
{
	if(argC == 1)
	{
		SocketCommons_SendMessageString((int)extraData, "Empezando comunicacion privada!");
		Logger_Log(LOG_DEBUG, "Empezando comunicacion privada!");
		while(1)
		{
			OnArrivedData* data = SocketServer_WakeMeUpWhenDataIsAvailableOn((int)extraData);

			if(data == NULL)
			{
				printf("!!!! terminando...\n");
				break;
			}

			printf("!!!! DESBLOQUEADO: leido '%d' bytes\n", data->receivedDataLength);

			printf("!!!!!!!! Recibido por privado: '%s'\n", (char*)data->receivedData);
			SocketCommons_SendMessageString((int)extraData, "OK!");
			if(StringUtils_CountOccurrences((char*)data->receivedData, "10") > 0)
			{
				free(data->receivedData);
				free(data);
				break;
			}
			free(data->receivedData);
			free(data);
		}
	}

	CommandInterpreter_FreeArguments(args);
	return 0;
}

void *CommandStopServer (int argC, char** args, char* callingLine, void* extraData)
{
	SocketServer_Stop();

	CommandInterpreter_FreeArguments(args);
	ThreadPool_FreeGracefully(pool);
	return 0;
}


void ClientConnected(int socket)
{
	printf("Cliente se conecto! %d\n", socket);
}

void ClientDisconnected(int socket)
{
	printf("Cliente se fue! %d\n", socket);
}

void ClientError(int socketID, int errorCode)
{
	printf("Cliente %d se reporto con error %s!\n", socketID, strerror(errorCode));
}


void* Server()
{
	CommandInterpreter_Init();
	pool = ThreadPool_CreatePool(10, false);
	CommandInterpreter_RegisterCommand("stop", (void*)CommandStopServer);
	CommandInterpreter_RegisterCommand("double", (void*)CommandDouble);
	CommandInterpreter_RegisterCommand("private", (void*)CommandPrivate);

	SocketServer_Start("CHAT", 8086);
	SocketServer_ActionsListeners actions = INIT_ACTION_LISTENER;

	actions.OnConsoleInputReceived = (void*)processLineInput;
	actions.OnPacketArrived = (void*)onPacketArrived;
	actions.OnClientConnected = (void*)ClientConnected;
	actions.OnClientDisconnect = (void*)ClientDisconnected;
	actions.OnReceiveError = (void*)ClientError;

	SocketServer_ListenForConnection(actions);
	printf("SERVER SHUTDOWN\n");
	//SocketServer_ListenForConnection(logger, 0);
	return NULL;
}

void* ClientManageReceptions(void* socket)
{
	char* res;
	int te, err, len;
	//while(1)
	{
		res = SocketCommons_ReceiveData( ((int)socket) , &te, &len, &err);
		if(res != 0)
			printf("Response received type %d '%s'!\n", te, (char*)res);
		else
			recibir = false;
	}

	return 0;
}

void Client(char* texto)
{
	printf("Conectando al server...\n");
	int sock = SocketClient_ConnectToServer("8086");
	printf("Socket asignado %d\n", sock);
	int m;
	pthread_t threadRecv;
	/*while(1)
	{
		if(!recibir)
			break;

		printf("WHILE\n");
		m = SocketCommons_SendMessageString(sock, texto);
		printf("String enviado. Retorno %d\n", m);

		ThreadManager_CreateThread(&threadRecv, ClientManageReceptions, ((void*) sock) );
		printf("Awaiting response...\n");
		pthread_join(threadRecv, NULL);

		sleep(1);
	}*/
	m = SocketCommons_SendMessageString(sock, texto);

	char* res;
	int te, err, len;
	//while(1)
	{
		res = SocketCommons_ReceiveData( sock , &te, &len, &err);
		if(res != 0)
			printf("Response received type %d '%s'!\n", te, (char*)res);
		else
			recibir = false;
	}

	printf("String enviado. Retorno %d\n", m);
	int i = 0;
	char* tosend;
	while(1)
	{
		if(!recibir)
			break;
		sleep(2);
		tosend = StringUtils_Format("%d", i);
		m = SocketCommons_SendMessageString(sock, tosend);
		printf("Elemento enviado. Retorno %d\n", m);

		res = SocketCommons_ReceiveData( sock , &te, &len, &err);
		if(res != 0)
			printf("Response received type %d '%s'!\n", te, (char*)res);
		else
			recibir = false;

		i++;

		sleep(1);
	}
}

void* ClientServer(void* port)
{
	printf("Conectando al server...\n");
	int sock = SocketClient_ConnectToServer( ((char*)port) );
	printf("Socket asignado %d\n", sock);
	int m;
	char* asd = (char*)malloc(1);
	while(1)
	{
		printf("WHILE\n");
		m = SocketCommons_SendMessageString(sock, "Hello World!");
		printf("String enviado. Retorno %d\n", m);
		int st = recv(sock, asd, 0, MSG_WAITALL);
		free(asd);
		printf("RECV = %d\n", st);
		if(st == 0)
			break;
		sleep(1);
	}

	return NULL;
}

void *CommandPrintF (int argC, char** args, char* callingLine, void* extraData)
{

	printf("Hola! Soy el handler de printf, me llego el comando '%s'\n", args[0]);

	for(int i = 1; i <= argC; i++)
	{
		printf("Argumento %d: %s\n", i, args[i]);
	}

	if(extraData != NULL)
		printf("Extra data: %s\n", (char*)extraData); //Ya sabia que era un char* por eso puedo castear alegremente.

	CommandInterpreter_FreeArguments(args);
	return 0;
}

void *CommandSum (int argC, char** args, char* callingLine, void* extraData)
{

	if(argC == 2)
	{
		int n1 = atoi(args[1]);
		int n2 = atoi(args[2]);

		printf("%d + %d = %d\n", n1, n2, n1+n2);
	}

	CommandInterpreter_FreeArguments(args);
	return 0;
}

void ProbarCommandInterpreter()
{
	Logger_CreateLog("./chatapp.log", "CHARAPP", true);

	CommandInterpreter_Init();

	CommandInterpreter_RegisterCommand("printf", (void*)CommandPrintF);
	CommandInterpreter_RegisterCommand("sum", (void*)CommandSum);

	int res;

	res = CommandInterpreter_Do("printf hola mundo!", " ", "Esto es cualquier info extra y podria ser hasta un struct!");

	printf("Resultado de comando 'printf' - registrado = %d.\n", res);

	res = CommandInterpreter_Do("alguncomando no registrado", " ", NULL);

	printf("Resultado de comando 'alguncomando' - registrado = %d.\n", res);

	CommandInterpreter_Do("sum 1 2", " ", NULL);

	//CommandInterpreter_Destroy(); No hace falta llamar a esto si usamos el exit_gracefully() de megekemmen.h

	exitok();
}

void* CountTo(void* args)
{
	for(int i = 0; i < 5; i++)
	{
		//printf("%d: %d\n", (int)args, i);
		sleep(1);
	}

	return 0;
}

void* SleepFor(void* args)
{
	sleep(10);

	return 0;
}

void ThreadPoolFunc()
{
	Logger_CreateLog("./chatapp.log", "CHARAPP", true);

	ThreadPool* pool = ThreadPool_CreatePool(10, false);

	ThreadPoolRunnable* run;

	for(int i = 0; i < 100; i++)
	{
		run = ThreadPool_CreateRunnable();
		run->data = (void*)i;
		if(i%2 == 0)
			run->runnable = (void*)CountTo;
		else
			run->runnable = (void*)SleepFor;
		ThreadPool_AddJob(pool, run);
	}

	//getchar();
	/*sleep(5);
	ThreadPool_ExitWhenPossibleAsync(pool);
	sleep(15);*/
	ThreadPool_FreeGracefully(pool);

	getchar();
	exitok();
}

void TestSerialization()
{
	Logger_CreateLog("./chatapp.log", "CHARAPP", true);

	int* i1 = (int*)malloc(4);
	*i1 = 1;
	char* i2 = (char*)malloc(6);
	strcpy(i2, "hola!");
	/**i2 = 2;
	int* i3 = (int*)malloc(4);
	*i3 = 3;
	int* i4 = (int*)malloc(4);
	*i4 = 4;*/

	SerializedPart p1 = {.size = 4, .data = i1};
	SerializedPart p2 = {.size = 6, .data = i2};

	void* packet = Serialization_Serialize(2, p1, p2);

	printf("OK\n");

	uint32_t size;
	memcpy(&size, packet + 4, sizeof(uint32_t));

	char* data = malloc(6);

	memcpy(data, packet + 12, 6);

	printf("size = %d, data = %s\n", size, data);

	free(packet);
	free(i1);
	free(i2);
	/*free(i3);
	free(i4);*/
	exitok();
}

void TestDeserialization()
{
	declare_and_init(i1, int, 114)

	char* i2 = (char*)malloc(6);
	strcpy(i2, "hola!");
	declare_and_init(i3, int, 114)
	declare_and_init(i4, int, 115)
	declare_and_init(i5, int, 116)
	/*i2 = 2;
	int* i3 = (int*)malloc(4);
	*i3 = 3;
	int* i4 = (int*)malloc(4);
	*i4 = 4;*/

	SerializedPart p1 = {.size = 4, .data = i1};
	SerializedPart p2 = {.size = 6, .data = i2};
	SerializedPart p3 = {.size = 4, .data = i3};
	SerializedPart p4 = {.size = 4, .data = i4};
	SerializedPart p5 = {.size = 4, .data = i5};

	SerializedPart* packet = Serialization_Serialize(5, p1, p2,p3,p4,p5);
	DeserializedData* arr = Serialization_Deserialize(packet->data);
	Serialization_CleanupSerializedPacket(packet);

	printf("FieldCount: %d\n", arr->count);
	printf("First field: %d\n", *((int*) arr->parts[0]));
	printf("Second field: %s\n", (char*) arr->parts[1]);
	printf("Third field: %d\n", *((int*) arr->parts[2]));
	printf("Fourth field: %d\n", *((int*) arr->parts[3]));
	printf("Fifth field: %d\n", *((int*) arr->parts[4]));
	Serialization_CleanupDeserializationStruct(arr);
	free(i1);
	free(i2);
	free(i3);
	free(i4);
	free(i5);
}

int main(int argc, char **argv)
{
	//ProbarCommandInterpreter();
	//ThreadPoolFunc();
	//TestSerialization();
	TestDeserialization();
	return 0;
	Logger_CreateLog("./chatapp.log", "CHARAPP", true);


	///////////////////////////////////////////////////////

	if(argc < 2)
	{
		printf("Debe ingresar el nombre de la instancia como parametro.\n");
		exitok();
	}

	Logger_Log(LOG_INFO, "Iniciando ChatApp con instance name %s...", argv[1]);

	if(strcmp("cli", argv[1]) == 0)
	{
		if(argc < 3)
		{
			printf("Debe ingresar el texto a mandar como parametro.\n");
			exitok();
		}

		Client(argv[2]);
		printf("Client end\n");
	}
	else
	{
		if(argc < 3)
		{
			Server();
		} else
		{
			pthread_t threadServer, threadClient;

			ThreadManager_CreateThread(&threadServer, Server, NULL);
			ThreadManager_CreateThread(&threadClient, ClientServer, ((void*) argv[2]) );
			printf("RUNNING THREADS\n");
			pthread_join(threadServer, NULL);
			pthread_join(threadClient, NULL);
			printf("JOINED BOTH\n");
		}
	}

	//Logger_DestroyLog(); No hace falta llamar a esto si usamos el exit_gracefully() de megekemmen.h

	exitok();
}
