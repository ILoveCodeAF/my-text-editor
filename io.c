/* my libs */
#include "io.h"
#include "line.h"
#include "te.h"
#include "x.h"

#include <time.h>
#include <stdio.h>

typedef struct _LINE_LINK_LIST LLL;
typedef struct _IO IO;
typedef struct _SHARED Shared;


/* globals */
IO io;
Shared* shared;

void
io_print()
{
	LLL* temp = io.root;
	printf("\n--start--\n");
	while(temp != NULL){// && line_length(&temp->line) != 0){
		line_print(&temp->line);
		temp = temp->next;
	}
	printf("\n--end--\n");
	return;
}

void
io_open_file(char* mode)
{
	io.file_pointer = fopen(io.filename, mode);
}

void
io_close_file()
{
	fclose(io.file_pointer); 
}

void
io_write_file()
{
	io_open_file("w");

	LLL* temp = io.root;
	int i = 0;
	while(temp != NULL){
		i = 0;
		while(i<line_length(&temp->line)){
			fputc(temp->line.characters[i], io.file_pointer);
			++i;
		}
		temp = temp->next;
	}
	io_close_file();
}

void
io_read_file()
{
	io_open_file("r");
	if(io.file_pointer != NULL){
		//do some thing
		io_close_file();
	}
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

void
io_put_char(char c)
{
	line_add_char(&io.current->line, c, io.io_input_cursor);
	io.io_input_cursor += 1;
	if(c == '\n'){
		io.current->next = (LLL*) malloc(sizeof(LLL));
		io.current->next->prev = io.current;
		io.current = io.current->next;
		line_init(&io.current->line);
		io.num_line++;
		io.io_input_cursor = 1;
	}
	//if c == newline
	//create new line
}

void
io_init(void* param)
{
	if(param != NULL)
		shared = (Shared*)param;

	io.root = (LLL*) malloc(sizeof(LLL));
	io.current = io.root;
	io.root->prev = io.root->next = NULL;
	line_init(&io.root->line);
	io.num_line = 1;
	io.io_input_cursor = 1;
}

void
io_run()
{
	io.filename = shared->filename;

	clock_t t1 = clock(), t2;
	int period = 200;// 200ms
	int div = CLOCKS_PER_SEC/1000;//ms
	
	char c;
	int not_break = 1;
	while(not_break){
		t2 = clock();
		if( (t2-t1)/div > 200){
			t1 = clock();
			shared_lock(shared);
			while(!queue_empty(&shared->buffer)){
				c = queue_pop(&shared->buffer);
				io_put_char(c);
//			if(ksym == XK_Escape){ //esc pressed
//				not_break = 0; //break
//				break;
//			}
				if(c == 27){//esc = 27
					not_break = 0;
					break;
				}
			}
			shared_unlock(shared);
		}
	}
}
