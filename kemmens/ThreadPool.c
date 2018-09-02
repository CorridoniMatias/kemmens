#include "kemmens/ThreadPool.h"

static void* ThreadPool_BasicOperation(void* ownerPool)
{
	ThreadPool* owner = (ThreadPool*)ownerPool;

	Logger_Log(LOG_DEBUG, "Thread Registering");

	while(!owner->terminate_all)
	{
		pthread_mutex_lock(&owner->lock);

		owner->free_threads++;

		if(queue_size(owner->jobs) == 0)
			pthread_cond_wait(&owner->cond, &owner->lock);

		if(queue_size(owner->jobs) > 0)
		{
			ThreadPoolRunnable* run = queue_pop(owner->jobs);
			owner->free_threads--;
			Logger_Log(LOG_DEBUG, "Working on job... Pool job queue count: %d. Free threads: %d", queue_size(owner->jobs), owner->free_threads);
			pthread_mutex_unlock(&owner->lock);

			run->runnable(run->data); //adentro de runnable se tiene que liberar la data! Responsabilidad del usuario!

			free(run);
			Logger_Log(LOG_DEBUG, "Thread Finished, free threads: %d", owner->free_threads+1);
			continue;
		}

		pthread_mutex_unlock(&owner->lock);
	}

	pthread_exit(0);
	return 0;
}

static void ThreadPool_CreateAndAddThreads(ThreadPool* pool, int amt, bool detached)
{
	pthread_t *thread;
	for(int i = 0; i < amt;i++)
	{
		thread = ThreadManager_MallocThread();

		if(detached)
			ThreadManager_CreateDetachedThread(thread, ThreadPool_BasicOperation, (void*) pool);
		else
			ThreadManager_CreateThread(thread, ThreadPool_BasicOperation, (void*) pool);

		list_add(pool->threads, thread);
	}
}

ThreadPool* ThreadPool_CreatePool(int threadAmount, bool detached)
{
	ThreadPool* tp = (ThreadPool*)malloc(sizeof(ThreadPool));

	tp->free_threads = 0;
	tp->jobs_available = false;
	tp->terminate_all = false;
	pthread_mutex_init(&(tp->lock), NULL);
	pthread_cond_init(&tp->cond, NULL);
	tp->threads = list_create();
	tp->jobs = queue_create();

	ThreadPool_CreateAndAddThreads(tp, threadAmount, detached);

	return tp;
}

void ThreadPool_AddJob(ThreadPool* pool, ThreadPoolRunnable* job)
{
	pthread_mutex_lock(&pool->lock);

	Logger_Log(LOG_DEBUG, "Adding job... Pool job queue count: %d", queue_size(pool->jobs));

	queue_push(pool->jobs, job);

	pthread_cond_broadcast(&pool->cond);
	pthread_mutex_unlock(&pool->lock);
}
