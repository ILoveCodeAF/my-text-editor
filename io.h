#ifndef __IO_H_INCLUDE__
#define __IO_H_INCLUDE__

#include "line.h"
#include "stdio.h"

typedef struct _LINE Line;

struct _LINE_LINK_LIST{
	Line line;
	struct _LINE_LINK_LIST* prev;
	struct _LINE_LINK_LIST* next;
	
};

struct _IO{
	int num_line;
	int io_input_cursor;
	struct _LINE_LINK_LIST* root;
	struct _LINE_LINK_LIST* current;
	FILE* file_pointer;
	char* filename;
};

void io_init(void* param);
void io_run();
//void io_open_file(char* mode);
//void io_write_file();
//void io_read_file();
//void io_close_file();
void io_put_char(char c);
void io_free();
void io_print();
#endif
