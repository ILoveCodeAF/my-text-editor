#ifndef __LINE_H_INCLUDE__
#define __LINE_H_INCLUDE__

struct _LINE{
	char* characters;
	int next;
	int max_size;
};

void line_init(struct _LINE* line);
void line_inc_max_size(struct _LINE* line);
int line_length(struct _LINE* line);
void line_add(struct _LINE* line, char c);
void line_add_char(struct _LINE* line, char c, int position);//count from 1;
void line_delete(struct _LINE* line);
void line_delete_char(struct _LINE* line, int position);// count from 1;
char line_get_char(struct _LINE* line, int position);
void line_print(struct _LINE* line);
void line_free(struct _LINE* line);

#endif
