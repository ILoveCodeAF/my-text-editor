#include <stdlib.h>
#include "unicode.h"
#include "stack.h"

typedef unsigned int uint;

char*
wchar_to_utf8(uint character)
{
	if(character < 128){
		char* c = (char*) malloc(sizeof(char));
		*c = (char)character;
		return c;
	}
	Stack stack;
	stack_init(&stack);
	int first_bit = 2;
	char c = 0;
	while(character >= ((1 << (8-first_bit)) - 1)){
		c = (((1<<6) - 1) & character) | (0x80);//128 || 1<<7
		character >>= 6;
		first_bit += 1;
		stack_push(&stack, c);
	}

	c = 0;
	int i = first_bit;
	while(i > 1){
		c <<= 1;
		c += 1;
		i -= 1;
	}
	c = (c << (8-(first_bit-1))) | character;
	char* ch = (char*) malloc(sizeof(stack_size(&stack))+1);
	i = 1;
	ch[0] = c;
	while(!stack_empty(&stack)){
		ch[i] = stack_pop(&stack);
		i++;
	}
	stack_free(&stack);
	return ch;
}

uint
utf8_to_wchar(char* character)
{
	int len = utf8_len_char(character);
	if(len == 1)
		return *character;
	uint wchar = 0;
	int mask = ((1 << (8 - (len + 1))) - 1);
	wchar =  mask & *character;
	int i = 1;
	mask = (1 << 6) - 1;
	while(i<len){
		wchar = (wchar << 6) | (mask & character[i]);
		i += 1;
	}
	if(i<len)
		return 0;
	return wchar;
}

char*
wchar_string_to_utf8_string(uint* characters)
{
}

uint*
utf8_string_to_wchar_string(char* characters)
{
}

uint
utf8_len_char(char* character)
{
	int mask = (1 << 7);// 128 || 0x80
	if(!(mask & *character))
		return 1;
	int len = 0;
	while(mask & *character){
		len += 1;
		mask >>= 1;
	}
	return len;
}
