#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

void
stack_init(Stack* stack)
{
	if(stack == NULL){
		printf("stack is NULL\n");
		return;
	}
	stack->top = -1;
//	stack->size = 0;
	stack->max_size = 1;
	stack->buffer = (_TYPE*) malloc(sizeof(_TYPE));
}

void
stack_free(Stack* stack)
{
	if(stack != NULL){
		if(stack->buffer != NULL){
			free(stack->buffer);
		}
	}
}

int
stack_empty(Stack* stack)
{
	return (stack->top == -1);
}

int
stack_full(Stack* stack)
{
	return (stack->top == stack->max_size-1);
}

void
stack_push(Stack* stack, _TYPE element)
{
	if(stack->top+1 == stack->max_size){
		stack_inc_max_size(stack);
	}
	stack->top += 1;
	stack->buffer[stack->top] = element;
}

_TYPE
stack_pop(Stack* stack)
{
	if(stack_empty(stack)){
		printf("stack is empty!!\n");
		return -1;
	}
	stack->top -= 1;
	return stack->buffer[stack->top + 1];
}

int
stack_size(Stack* stack)
{
	return stack->top+1;
}

void
stack_inc_max_size(Stack* stack)
{
	stack->max_size <<= 1;
	_TYPE* temp = (_TYPE*) malloc (sizeof(_TYPE)*stack->max_size);
	int i = 0;
	while(i<=stack->top){
		temp[i] = stack->buffer[i];
		++i;
	}
	free(stack->buffer);
	stack->buffer = temp;
}
