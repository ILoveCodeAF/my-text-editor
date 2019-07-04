#ifndef __IO_H_INCLUDE__
#define __IO_H_INCLUDE__

#include "line.h"

typedef struct _LINE Line;

struct _LINE_LINK_LIST{
	Line line;
	struct _LINE_LINK_LIST* prev;
	struct _LINE_LINK_LIST* next;
};

struct _IO{
	int io_input_cursor;
	struct _LINE_LINK_LIST* root;
	struct _LINE_LINK_LIST* current;
};

void io_init();
void io_run(void* param);
void io_open_file();
void io_write_file();
void io_close_file();
void io_free();

#endif
