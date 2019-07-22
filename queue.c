#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _QUEUE queue;
/* define type of elements */
typedef int _TYPE;

void
queue_init(queue* q, int size)
{
	if(q == NULL){
		printf("error: NULL poiter!!\n");
		exit(1);
	}
	q->front = 0;
	q->back = 1;
	q->max_size = size;
	q->size = 0;
	q->buffer = (int*) malloc(sizeof(int)*size);
}

void
queue_destroy(queue* q)
{
	if(q->buffer != NULL)
		free(q->buffer);
}

int
queue_empty(queue* q)
{
//if(q->front+1 == q->back)
//	return 1;
//if(q->front == q->max_size-1 && q->back == q->max_size)
//	return 1;
//return 0;
	return (q->size == 0)? 1:0;
}

int
queue_full(queue* q)
{
//if(q->front == q->back+1)
//	return 1;
//if(q->front == 0 && q->back == q->max_size)
//	return 1;
//return 0;
	return (q->size == q->max_size-2)? 1:0;
}

void
queue_push(queue* q, _TYPE n)
{
	if(!queue_full(q)){
		if(q->back == q->max_size){
			q->back = 0;
		}
		q->buffer[q->back] = n;
		q->back += 1;
		q->size += 1;
	}
}

_TYPE
queue_pop(queue* q)
{
	if(!queue_empty(q)){
		if(q->front == q->max_size-1){
			q->front = 0;
		}
		else
			q->front += 1;
		q->size -= 1;
		return q->buffer[q->front];
	}
	return -1;
}

int
queue_size(queue* q)
{
	return q->size;
}
