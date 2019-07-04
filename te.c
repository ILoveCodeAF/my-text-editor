/* my libs */
#include "te.h"
#include "x.h"
#include "queue.h"
#include "io.h"

//#include <unistd.h> //sleep
#include <pthread.h>

typedef struct _SHARED Shared;

/* globals */
Shared shared_global;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void
shared_lock(Shared* shared)
{
	pthread_mutex_lock(shared->mutex);
}

void
shared_unlock(Shared* shared)
{
	pthread_mutex_unlock(shared->mutex);
}

//queue buffer;

void*
xAPI(void* param)
{
	printf("\nRunning thread xAPI...\n");
	xinit();
	xrun(param);
	xfree();
	xclose();
	printf("\nClosing thread xAPI...\n");
}

void*
ioAPI(void* param)
{
	printf("\nRunning thread ioAPI...\n");
	io_init();
	io_run(param);
	io_free();
	printf("\nclosing thread ioAPI...\n");
}


int
main()
{
	shared_global.mutex = &mutex;
	queue_init(&shared_global.buffer, 50);

	pthread_t thread_xapi_id;
	pthread_t thread_ioapi_id;

	printf("\nCreating thread xAPI...\n");	
	pthread_create(&thread_xapi_id, NULL, xAPI, &shared_global);

	printf("\nCreating thread ioAPI..\n");
	pthread_create(&thread_ioapi_id, NULL, ioAPI, &shared_global);
	pthread_join(thread_ioapi_id, NULL);

	
	return 0;
}
