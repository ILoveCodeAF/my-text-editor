#ifndef __QUEUE_H__
#define __QUEUE_H__

struct _QUEUE{
 	int front;
	int back;
	int* buffer;
	int max_size;
	int size;
};
void queue_init(struct _QUEUE*, int);
void queue_destroy(struct _QUEUE*);
int queue_empty(struct _QUEUE*);
int queue_full(struct _QUEUE*);
void queue_push(struct _QUEUE*, int);
int queue_pop(struct _QUEUE*);
int queue_size(struct _QUEUE*);

#endif
