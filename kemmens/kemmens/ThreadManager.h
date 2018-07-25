#ifndef KEMMENS_THREADMANAGER_H_
#define KEMMENS_THREADMANAGER_H_

#include <pthread.h>
#include <stdio.h>
#include <commons/log.h>

/*	Mallocea un pthread_t.
 *
 * OJO!! Despues de hacer un join al thread creado se DEBE hacer el free
 * 			Si el thread se detachea no se que pasa!
 *
 */
pthread_t* ThreadManager_MallocThread();

/*	Crea un thread que no esta detached.
 */
int ThreadManager_CreateThread(t_log* logger, pthread_t* thread, void *(*startingPoint) (void *), void *__restrict argument);

/*	Crea un thread y hace el detach del mismo.
 *
 */
int ThreadManager_CreateDetachedThread(t_log* logger, pthread_t* thread, void *(*startingPoint) (void *), void *__restrict argument);

#endif /* KEMMENS_THREADMANAGER_H_ */
