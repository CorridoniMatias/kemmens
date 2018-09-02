#ifndef KEMMENS_THREADPOOL_H_
#define KEMMENS_THREADPOOL_H_

#include "ThreadManager.h"
#include "commons/collections/list.h"
#include "commons/collections/queue.h"

struct ThreadPool_func_s
{
	void* data;
	void (*runnable)(void* args);
} typedef ThreadPoolRunnable;

struct ThreadPool_s
{
	bool terminate_all;
	bool jobs_available;
	int free_threads;
	t_list *threads;
	t_queue *jobs;
	pthread_cond_t cond;
	pthread_mutex_t lock;
}typedef ThreadPool;


void ThreadPool_AddJob(ThreadPool* pool, ThreadPoolRunnable* job);
ThreadPool* ThreadPool_CreatePool(int threadAmount, bool detached);


#endif /* KEMMENS_THREADPOOL_H_ */
