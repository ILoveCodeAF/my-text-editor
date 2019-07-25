#ifndef __LINE_H_INCLUDED__
#define __LINE_H_INCLUDED__

typedef struct _LINE{
	char* characters;
	int next;
	int max_size;
} Line;

void line_init(Line* line);
void line_inc_max_size(Line* line);
int line_length(Line* line);
void line_add(Line* line, char c);
void line_add_char(Line* line, char c, int position);//count from 1;
void line_delete(Line* line);
void line_delete_char(Line* line, int position);// count from 1;
char line_get_char(Line* line, int position);
void line_print(Line* line);
void line_free(Line* line);

#endif
