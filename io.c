/* my libs */
#include "io.h"
#include "line.h"
#include "x.h"

#include <time.h>
#include <stdio.h>

//typedef struct _LINE_LINK_LIST LLL;
//typedef struct _IO IO;
//typedef struct _SHARED Shared;



void
io_handle_char(IO* io, char c)
{
//if( c == 27 )
//	return;	
	if( c == '\r' )//return | enter
		c = '\n';
	if( c == '\b' ){//backspace
		if(line_length(&io->current->line) > 0){
			line_delete(&io->current->line);
			io->input_cursor -= 1;
		}
		else{
			if(io->current->prev != NULL){
				io->current = io->current->prev;
				free(io->current->next);
				io->current->next = NULL;
				line_delete(&io->current->line);
				io->input_cursor = io->current->line.next + 1;
			}
		}
		return;
	}
	line_add_char(&io->current->line, c, io->input_cursor);
	io->input_cursor += 1;
	if(c == '\n'){
		io->current->next = (LLL*) malloc(sizeof(LLL));
		io->current->next->prev = io->current;
		io->current = io->current->next;
		io->current->next = NULL;
		line_init(&io->current->line);
		io->num_line++;
		io->input_cursor = 1;
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

int
io_length_line(IO* io, int line)
{
	LLL* temp = io->current;
	if(line > 0){
		while(line != 0 && temp->next != NULL){
			line -= 1;
			temp = temp->next;
		}
		if(line != 0)
			return -1;
	}
	if(line < 0){
		while(line != 0 && temp->prev != NULL){
			line += 1;
			temp = temp->prev;
		}
		if(line != 0)
			return -1;
	}
	return line_length(&temp->line);
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
