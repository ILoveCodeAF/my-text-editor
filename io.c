/* my libs */
#include "io.h"
#include "te.h"
#include "x.h"

#include <time.h>
#include <stdio.h>
#include <X11/Xlib.h>

typedef struct _LINE_LINK_LIST LLL;
typedef struct _IO IO;
typedef struct _SHARED Shared;


/* globals */
IO io;

void
io_init()
{
	io.io_input_cursor = 0;
	io.root = (LLL*) malloc(sizeof(LLL));
	io.current = io.root;
	io.root->prev = io.root->next = NULL;
}

void
io_run(void* param)
{
    Shared* shared = (Shared*) param;
	
	clock_t t1 = clock(), t2;
	int period = 200;// 200ms
	int div = CLOCKS_PER_SEC/1000;//ms
	
	int  ksym;
	int not_break = 1;
	while(not_break){
		t2 = clock();
		if( (t2-t1)/div > 200){
			t1 = clock();
			shared_lock(shared);
			while(!queue_empty(&shared->buffer)){
				ksym = queue_pop(&shared->buffer);
				if(ksym>31 && ksym<127){	
					printf("%c", (char)ksym);
				}
				if(ksym == XK_Escape){ //esc pressed
					not_break = 0; //break
					break;
				}
			}
			shared_unlock(shared);
		}
	}
}

void
io_open_file()
{

}

void
io_close_file()
{

}

void
io_write_file()
{

}

void
io_free()
{
	while(io.root->next != NULL){
		io.root = io.root->next;
		free(io.root->prev);
	}
	free(io.root);
}
