#include "kemmens/ThreadManager.h"

pthread_t* ThreadManager_MallocThread()
{
	return (pthread_t*)malloc(sizeof(pthread_t));
}

int ThreadManager_CreateThread(pthread_t* thread, void *(*startingPoint) (void *), void *__restrict argument)
{
	int status = pthread_create(thread, NULL, (void*) startingPoint, (void*) argument);

	#ifndef KEMMENS_DISABLE_LOGGING
	#ifndef THREADMANAGER_DISABLE_LOGGING
	if(status != 0)
		Logger_Log(LOG_ERROR, "KEMMENSLIB::THREADMANAGER->CREATETHREAD - Error al crear thread. Status = '%d'", status);
	#endif
	#endif
	return status;
}

int ThreadManager_CreateDetachedThread(pthread_t* thread, void *(*startingPoint) (void *), void *__restrict argument)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	int status = pthread_create(thread, &attr, startingPoint, argument);
	pthread_attr_destroy(&attr);

	#ifndef KEMMENS_DISABLE_LOGGING
	#ifndef THREADMANAGER_DISABLE_LOGGING
	if(status != 0)
		Logger_Log(LOG_ERROR, "KEMMENSLIB::THREADMANAGER->CREATEDETACHEDTHREAD - Error al crear detached thread. Status = '%d'", status);
	#endif
	#endif

	return status;
}
