#ifndef __TE_H_INCLUDE__
#define __TE_H_INCLUDE__

#include <pthread.h>
#include "queue.h"

struct _SHARED {
	pthread_mutex_t* mutex;
	struct _QUEUE buffer;
	char* filename;
};

void shared_lock(struct _SHARED*);
void shared_unlock(struct _SHARED*);

#endif
