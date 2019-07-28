/* my libs */
#include "io.h"
#include "line.h"
#include "x.h"
#include "unicode.h"

#include <time.h>
#include <stdio.h>

//typedef struct _LINE_LINK_LIST LLL;
//typedef struct _IO IO;
//typedef struct _SHARED Shared;

// #define BYTE_PATTERN "%c%c%c%c%c%c%c%c"
// #define BYTE_BINARY(byte) \
// 	(byte & 0x80? '1':'0'), \
// 	(byte & 0x40? '1':'0'), \
// 	(byte & 0x20? '1':'0'), \
// 	(byte & 0x10? '1':'0'), \
// 	(byte & 0x08? '1':'0'), \
// 	(byte & 0x04? '1':'0'), \
// 	(byte & 0x02? '1':'0'), \
// 	(byte & 0x01? '1':'0')

void
io_backspace(IO* io)
{
	if(line_length(&io->current->line) > 0 && io->input_cursor > 1){
		// printf(BYTE_PATTERN"\n", BYTE_BINARY(line_get_char(&io->current->line, io->input_cursor-1)));
		// printf("next:%d io %d\n", io->current->line.next, io->input_cursor);
		if(line_get_char(&io->current->line, io->input_cursor-1) & 0x80)
		{
			while((line_get_char(&io->current->line, io->input_cursor-1) 
					& 0x40) == 0){
				line_delete_char(&io->current->line, io->input_cursor-1);
				io->input_cursor -= 1;
			}
			// printf("y\n");
			line_delete_char(&io->current->line, io->input_cursor-1);
			io->input_cursor -= 1;
		}
		else{
			line_delete(&io->current->line);
			io->input_cursor -= 1;
		}
	}
	else{
		if(io->current->prev != NULL){
			io->current = io->current->prev;
			free(io->current->next);
			io->current->next = NULL;
			line_delete(&io->current->line);
			io->input_cursor = io->current->line.next + 1; //bugs
		}
	}
}

void
io_newline(IO* io)
{
	io->current->next = (LLL*) malloc(sizeof(LLL));
	io->current->next->prev = io->current;
	io->current = io->current->next;
	io->current->next = NULL;
	line_init(&io->current->line);
	io->num_line++;
	io->input_cursor = 1;
}

void
io_handle_char(IO* io, char c)
{
//if( c == 27 )
//	return;	
	if( c == '\r' )//return | enter
		c = '\n';
	if( c == '\b' ){//backspace
		io_backspace(io);
		return;
	}
	line_add_char(&io->current->line, c, io->input_cursor);
	io->input_cursor += 1;
	if(c == '\n'){
		io_newline(io);
	}
	//if c == newline
	//create new line
}

void
io_print(IO* io)
{
	LLL* temp = io->root;
	printf("\n--start--\n");
	while(temp != NULL){// && line_length(&temp->line) != 0){
		line_print(&temp->line);
		temp = temp->next;
	}
	printf("\n--end--\n");
	return;
}

void
io_open_file(IO* io, char* mode)
{
	io->file_pointer = fopen(io->filename, mode);
}

void
io_close_file(IO* io)
{
	fclose(io->file_pointer); 
}

void
io_write_file(IO* io)
{
	io_open_file(io, "wb");

	LLL* temp = io->root;
	while(temp != NULL && line_length(&temp->line) > 0){
		fwrite(temp->line.characters, line_length(&temp->line), 
				1, io->file_pointer);
		temp = temp->next;
	}
	io_close_file(io);
}

void
io_read_file(IO* io)
{
	io_open_file(io, "rb");
	if(io->file_pointer != NULL){
		int nread;
		int c;
		while((nread = fread(&c, 1, 1, io->file_pointer)) > 0){
			io_handle_char(io, c);
		}
		io_close_file(io);
	}
}

void
io_free(IO* io)
{
	while(io->root->next != NULL){
		io->root = io->root->next;
		free(io->root->prev);
	}
	free(io->root);
}

void
io_init(IO* io, char* filename)
{
	io->filename = filename;
	io->root = (LLL*) malloc(sizeof(LLL));
	io->current = io->root;
	io->root->prev = io->root->next = NULL;
	line_init(&io->root->line);
	io->num_line = 1;
	io->input_cursor = 1;
	io_read_file(io);

	LLL* temp = io->root;
	while(temp != NULL && line_length(&temp->line) > 0){
		xwrite(line_get_chars(&temp->line), line_length(&temp->line));
		temp = temp->next;
	}
	temp = NULL;
}

void
io_write(IO* io, char* buffer, int len)
{
	int i = 0;
	while(i < len){
		io_handle_char(io, buffer[i]);
		i += 1;
	}
}

int
io_length_line(IO* io, int line)
{
	Line* l = io_get_line(io, line);
	if(l == NULL)
		return -1;
	return line_length(l);
}

int
io_num_chars_of_line(IO* io, int line, int position)
{
	Line* l = io_get_line(io, line);
	if(l == NULL)
		return -1;
	int num_char = 0;
	char* c = line_get_chars(l);

	int i = 0;
	int len_char = 0;
	while(i < position-1){
		len_char = utf8_len_char(c+i);
		i += len_char;
		num_char += 1;
	}
	return num_char;
}

int
str_cmp(char* s1, char* s2, int len){
	int i = 0;
	while(i < len){
		if(*(s1+i) != *(s2+i))
			return 0;
		++i;
	}
	return 1;
}

int
io_num_char_of_line(IO* io, int line, char* c, int len, int position)
{
	Line* l = io_get_line(io, line);
	if(l == NULL)
		return -1;
	int num_char = 0;
	char* str = line_get_chars(l);
	int i = 0;
	int len_char = 0;
	while(i < position-1){
		len_char = utf8_len_char(str+i);
		if(len_char == len){
			if(str_cmp(str+i, c, len))
				num_char++;
		}
		i += len_char;
	}
	return num_char;
}

Line*
io_get_line(IO* io, int line)
{
	LLL* temp = io->current;
	if(line > 0){
		while(line != 0 && temp->next != NULL){
			temp = temp->next;
			line -= 1;
		}
		if(line != 0)
			return NULL;
	}
	else if(line < 0){
		while(line != 0 && temp->prev != NULL){
			temp = temp->prev;
			line += 1;
		}
		if(line != 0)
			return NULL;
	}
	return &temp->line;
}

char
io_get_byte(IO* io)
{
	if(io->input_cursor < 2)
		return 0;
	return io->current->line.characters[io->input_cursor-2];
}
