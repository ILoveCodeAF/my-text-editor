#ifndef __TE_H_INCLUDE__
#define __TE_H_INCLUDE__

#include <pthread.h>
#include "queue.h"

struct _BUFFER_THREAD_MUTEX {
	pthread_mutex_t* mutex;
	struct _QUEUE q;
};

#endif
