#ifndef KEMMENS_THREADPOOL_H_
#define KEMMENS_THREADPOOL_H_

#include "ThreadManager.h"
#include "commons/collections/list.h"
#include "commons/collections/queue.h"

//Para deshabilitar el logueo interno de este modulo de las kemmens descomentar la siguiente linea:
#define THREADPOOL_DISABLE_LOGGING


/**
 * 		Estructura que representa un job o trabajo a depositar en el pool
 * 		CAMPOS:
 * 			free_data: Funcion que nos indica como deberiamos hacer un free(data) en caso que se llame a la funcion
 * 			           para limpiar todos los trabajos del thread pool y este todavia no haya ejecutado,
 * 			           esto es porque si lo que esta en data no es algo del heap -> seg fault.
	                   En caso que free_data != NULL => se va a ejecutar esta funcion para liberar data.
	                   De lo contrario no se hace nada
 * 			data: Datos del trabajo, seran tomados como parametros por la funcion
 * 			runnable: Funcion a ejecutar al llevar a cabo el trabajo; recibe por parametro un cierto argumento
 */
struct ThreadPool_func_s
{
	/*free_data nos indica como deberiamos hacer un free(data) en caso que se llame a la funcion para limpiar todos los trabajos, esto es porque si lo que esta en data no es algo del heap -> seg fault.
	en caso que free_data != NULL => se va a ejecutar esta funcion para liberar data. De lo contrario no se hace nada*/
	void (*free_data)(void* data);
	void* data;
	void (*runnable)(void* args);
} typedef ThreadPoolRunnable;

/**
 * 		Estructura que representa un pool de threads (pozo de hilos) donde hay hilos disponibles a la espera de jobs a ejecutar
 * 		CAMPOS:
 * 			terminate_all: Flag que indica si es necesario terminar todos los hilos y jobs del pool
 * 			detached: Flag que indica si los hilos a crear en el pool son detacheados
 * 			free_threads: Cantidad de hilos libres/disponibles en el pool
 * 			threads: Lista de hilos del pool
 * 			jobs: Cola de jobs/trabajos pendientes, sin hilo asignado aun
 * 			cond: Variable de condicion, representa la condicion a evaluar para bloquear/desbloquear el mutex
 * 			lock: Semaforo mutex para evitar la concurrencia en la llegada de varios jobs
 */
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
