#ifndef __QUEUE_H_INCLUDED__
#define __QUEUE_H_INCLUDED__

/* define type of elements */
typedef int _TYPE;

typedef struct _QUEUE{
 	int front;
	int back;
	_TYPE* buffer;
	int max_size;
	int size;
} Queue;
void queue_init(Queue*, int);
void queue_destroy(Queue*);
int queue_empty(Queue*);
int queue_full(Queue*);
void queue_push(Queue*, _TYPE);
_TYPE queue_pop(Queue*);
int queue_size(Queue*);

#endif
