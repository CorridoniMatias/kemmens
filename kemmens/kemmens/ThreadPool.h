#ifndef KEMMENS_THREADPOOL_H_
#define KEMMENS_THREADPOOL_H_

#include "ThreadManager.h"
#include "commons/collections/list.h"
#include "commons/collections/queue.h"

struct ThreadPool_func_s
{
	//should_free_data nos indica si deberiamos hacer un free(data) en caso que se llame a la funcion para limpiar todos los trabajos, esto es porque si lo que esta en data no es algo del heap -> seg fault.
	bool should_free_data;
	void* data;
	void (*runnable)(void* args);
} typedef ThreadPoolRunnable;

struct ThreadPool_s
{
	bool terminate_all;
	bool detached;
	int free_threads;
	t_list *threads;
	t_queue *jobs;
	pthread_cond_t cond;
	pthread_mutex_t lock;
}typedef ThreadPool;

/**
 * 		Agrega un trabajo al pool
 */
void ThreadPool_AddJob(ThreadPool* pool, ThreadPoolRunnable* job);

/**
 *		Crea el Thread Pool.
 */
ThreadPool* ThreadPool_CreatePool(int threadAmount, bool detached);

/**
 * 		Elimina todos los jobs encolados que aun no se ejecutaron. Esto no interrumpe la ejecucion de los threads.
 */
void ThreadPool_ClearPendingJobs(ThreadPool* pool);

/**
 * 		Espera a que los threads terminen con su trabajo actual si es que estan haciendo alguno, elimina la lista de threads, saca todos los trabajos de la cola y libera toda la memoria usada.
 */
void ThreadPool_FreeGracefully(ThreadPool* pool);

/**
 * 		Les inidica a los Threads que luego que finalicen su procesamiento actual, no hagan mas nada.
 *
 * 		Esta funcion ESPERA a que los threads terminen, los joinea.
 */
void ThreadPool_ExitWhenPossibleWait(ThreadPool* pool);

/**
 * 		Les inidica a los Threads que luego que finalicen su procesamiento actual, no hagan mas nada.
 */
void ThreadPool_ExitWhenPossibleAsync(ThreadPool* pool);

/***
 * 		Malloc e inicializacion de la estructura de un job.
 */
ThreadPoolRunnable* ThreadPool_CreateRunnable();

/**
 *		Joinea todos los threads. No tiene mucho sentido a menos que se toque a mano terminate_all
 */
void ThreadPool_JoinAllThreads(ThreadPool* pool);

#endif /* KEMMENS_THREADPOOL_H_ */
