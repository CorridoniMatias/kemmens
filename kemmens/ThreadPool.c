#include "kemmens/ThreadPool.h"

static void* ThreadPool_BasicOperation(void* ownerPool)
{
	ThreadPool* owner = (ThreadPool*)ownerPool;

	#ifndef KEMMENS_DISABLE_LOGGING
	#ifndef THREADPOOL_DISABLE_LOGGING
	pthread_t this = pthread_self();
	Logger_Log(LOG_DEBUG, "KEMMENSLIB::ThreadPool->Thread %p Registering", &this);
	#endif
	#endif
	while(!owner->terminate_all)
	{
		pthread_mutex_lock(&owner->lock);

		if(queue_size(owner->jobs) == 0)
			pthread_cond_wait(&owner->cond, &owner->lock);

		if(owner->terminate_all)
		{
			pthread_mutex_unlock(&owner->lock);
			break;
		}

		if(queue_size(owner->jobs) > 0)
		{
			ThreadPoolRunnable* run = queue_pop(owner->jobs);
			owner->free_threads--;
			#ifndef KEMMENS_DISABLE_LOGGING
			#ifndef THREADPOOL_DISABLE_LOGGING
			Logger_Log(LOG_DEBUG, "KEMMENSLIB::ThreadPool->Thread %p working on job... Pool job queue count: %d. Free threads: %d", &this, queue_size(owner->jobs), owner->free_threads);
			#endif
			#endif
			pthread_mutex_unlock(&owner->lock);

			run->runnable(run->data); //adentro de runnable se tiene que liberar la data! Responsabilidad del usuario!

			free(run);
			pthread_mutex_lock(&owner->lock);
			owner->free_threads++;
			pthread_mutex_unlock(&owner->lock);

			#ifndef KEMMENS_DISABLE_LOGGING
			#ifndef THREADPOOL_DISABLE_LOGGING
			Logger_Log(LOG_DEBUG, "KEMMENSLIB::ThreadPool->Thread %p Finished, free threads: %d", &this, owner->free_threads);
			#endif
			#endif
			continue;
		}

		pthread_mutex_unlock(&owner->lock);
	}
	#ifndef KEMMENS_DISABLE_LOGGING
	#ifndef THREADPOOL_DISABLE_LOGGING
	Logger_Log(LOG_DEBUG, "KEMMENSLIB::ThreadPool->Thread %p EXITING!", &this);
	#endif
	#endif
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

	tp->free_threads = threadAmount;
	tp->detached = detached;
	tp->terminate_all = false;
	pthread_mutex_init(&(tp->lock), NULL);
	pthread_cond_init(&tp->cond, NULL);
	tp->threads = list_create();
	tp->jobs = queue_create();

	ThreadPool_CreateAndAddThreads(tp, threadAmount, detached);

	return tp;
}

ThreadPoolRunnable* ThreadPool_CreateRunnable()
{
	ThreadPoolRunnable* run = (ThreadPoolRunnable*)malloc(sizeof(ThreadPoolRunnable));

	run->data = NULL;
	run->runnable = NULL;
	run->free_data = NULL;

	return run;
}

void ThreadPool_AddJob(ThreadPool* pool, ThreadPoolRunnable* job)
{
	pthread_mutex_lock(&pool->lock);

	if(!pool->terminate_all)
	{
		#ifndef KEMMENS_DISABLE_LOGGING
		#ifndef THREADPOOL_DISABLE_LOGGING
		Logger_Log(LOG_DEBUG, "KEMMENSLIB::ThreadPool->Adding job... Pool job queue count: %d", queue_size(pool->jobs));
		#endif
		#endif
		queue_push(pool->jobs, job);

		pthread_cond_broadcast(&pool->cond);
	}

	pthread_mutex_unlock(&pool->lock);
}

void ThreadPool_ExitWhenPossibleAsync(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->lock);
	pool->terminate_all = true;
	pthread_cond_broadcast(&pool->cond);
	pthread_mutex_unlock(&pool->lock);
}

void ThreadPool_ExitWhenPossibleWait(ThreadPool* pool)
{
	ThreadPool_ExitWhenPossibleAsync(pool);

	ThreadPool_JoinAllThreads(pool);
}

static void ThreadPool_RemoveAllThreads(ThreadPool* pool)
{
	ThreadPool_ExitWhenPossibleWait(pool);

	list_destroy_and_destroy_elements(pool->threads, (void*)free);
}

void ThreadPool_FreeGracefully(ThreadPool* pool)
{
	ThreadPool_RemoveAllThreads(pool);
	ThreadPool_ClearPendingJobs(pool);
	queue_destroy(pool->jobs);
	free(pool);
}

void ThreadPool_JoinAllThreads(ThreadPool* pool)
{
	void joinAll(void* thread)
	{
		#ifndef KEMMENS_DISABLE_LOGGING
		#ifndef THREADPOOL_DISABLE_LOGGING
		Logger_Log(LOG_DEBUG, "KEMMENSLIB::ThreadPool->ThreadPool_ExitWhenPossibleWait - Joining thread %p...", thread);
		#endif
		#endif
		pthread_join( *((pthread_t*)thread), NULL );
	}

	if(!pool->detached)
		list_iterate(pool->threads, joinAll);
}

void ThreadPool_ClearPendingJobs(ThreadPool* pool)
{
	void destroyJob(void* job)
	{
		ThreadPoolRunnable* actual = (ThreadPoolRunnable*)job;

		if(actual->free_data != NULL && actual->data != NULL)
		{
			//SI el thread que esta corriendo esto actualmente es detached y le liberamos la memoria que esta usando puede causar un seg. fault!
			//El usuario tiene que especificar como se va a liberar la memoria de lo que sea que esta aca adentro.
			actual->free_data(actual->data);
		}

		if(actual)
			free(actual);

		#ifndef KEMMENS_DISABLE_LOGGING
		#ifndef THREADPOOL_DISABLE_LOGGING
		Logger_Log(LOG_DEBUG, "KEMMENSLIB::ThreadPool->ThreadPool_ClearPendingJobs - Removed job %p", job);
		#endif
		#endif
	}

	pthread_mutex_lock(&pool->lock);
	queue_clean_and_destroy_elements(pool->jobs, destroyJob);

	pthread_mutex_unlock(&pool->lock);
}



