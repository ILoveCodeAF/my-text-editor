#ifndef __STACK_H_INCLUDED__
#define __STACK_H_INCLUDED__

typedef int _TYPE;

typedef struct __STACK {
	int top;
//	int size;
	int max_size;
	_TYPE* buffer;
} Stack;

void stack_init(Stack*);
void stack_free(Stack*);
int stack_empty(Stack*);
int stack_full(Stack*);
void stack_push(Stack*, _TYPE);
_TYPE stack_pop(Stack*);
int stack_size(Stack*);
void stack_inc_max_size(Stack*);

#endif
