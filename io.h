#ifndef __IO_H_INCLUDED__
#define __IO_H_INCLUDED__

#include "line.h"
#include "stdio.h"

//typedef struct _LINE Line;

typedef struct _LINE_LINK_LIST{
	Line line;
	struct _LINE_LINK_LIST* prev;
	struct _LINE_LINK_LIST* next;
	
} LLL;

typedef struct _IO{
	int num_line;
	int input_cursor;
	struct _LINE_LINK_LIST* root;
	struct _LINE_LINK_LIST* current;
	FILE* file_pointer;
	char* filename;
} IO;

void io_init(IO* io, char* filename);
void io_open_file(IO* io, char* mode);
void io_write_file(IO*);
void io_read_file(IO*);
void io_close_file(IO*);
void io_handle_char(IO* io, char c);
void io_free(IO* io);
void io_print(IO* io);
int io_length_line(IO* io, int line);// 0 = current, 1 = next, -1 = prev line //return -1 (line do not exist)
void io_write(IO* io, char* buffer, int len);
//int io_num_chars(IO*);//current line from begin to cursor
int io_num_chars_of_line(IO* io, int line, int position);// 0 = current, 1 = nest -1 = prev, return -1 if( line do not exist) 
int io_num_char_of_line(IO* io, int line, char* c, int len, int position);
Line* io_get_line(IO* io, int line); //0 = current, 1 = next, -1 = prev ..; return null if not exist
char io_get_byte(IO* io);

#endif
