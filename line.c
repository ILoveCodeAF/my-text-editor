#include "line.h"
#include "stdio.h"
#include "stdlib.h"

//typedef struct _LINE Line;

void
line_init(Line* line)
{
	if(line == NULL){
		printf("line_error: null poiter!\n");
		exit(1);
	}
	line->max_size = 1;
	line->next = 0;
	line->characters = (char*)malloc(line->max_size*sizeof(char));
//int i = 0;
//while(i < line->max_size){
//	line->characters[i] = 0;
//	++i;
//}
}

void
line_inc_max_size(Line* line)
{
	line->max_size <<= 1;
	char* temp = (char*) malloc(line->max_size*sizeof(char));
	int i = 0;
	while(i<line->next){
		temp[i] = line->characters[i];
		++i;
	}
//while(i<line->max_size){
//	temp[i] = 0;
//	++i;
//}
	free(line->characters);
	line->characters = temp;
	temp = NULL;
}

int
line_length(Line* line)
{
	return line->next;
}

char*
line_get_chars(Line* line)
{
	return line->characters;
}

void
line_add(Line* line, char c)
{
	line_add_char(line, c, line->next+1);
}

void
line_add_char(Line* line, char c, int position)
{
	if(position-1 > line->next){
		printf("line_warning: out of line!\n");
		return;
	}
	if(line->next == line->max_size)
		line_inc_max_size(line);
	if(position-1 < line->next){
		int i = line->next;
		while(i>=position){
			line->characters[i] = line->characters[i-1];
			--i;
		}	
	}
	line->characters[position-1] = c;
	line->next += 1;
}

void
line_delete(Line* line)
{
	line_delete_char(line, line->next);
}

void
line_delete_char(Line* line, int position)
{
	if(position < 1)
		return;
	if(position-1 < line->next){
		while(position-1 < line->next){
			line->characters[position-1] = line->characters[position];
			position += 1;
		}
		line->next -= 1;
	}
}

char
line_get_char(Line* line, int position)
{
	if(position < 1 || position > line->next)
		return 0;
	return line->characters[position-1];
}

void
line_print(Line* line)
{
//int i = 0;
//while(i<line->next){
//	printf("%c", line->characters[i]);
//	++i;
//}
	printf("%.*s", line_length(line), line->characters);
}

void
line_free(Line* line)
{
	free(line->characters);
}

