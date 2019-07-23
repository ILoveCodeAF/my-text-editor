// https://en.wikipedia.org/wiki/UTF-8#Examples
#ifndef __UNICODE_H_INCLUDED__
#define __UNICODE_H_INCLUDED__

typedef unsigned int uint;

char* wchar_to_utf8(uint character);
uint utf8_to_wchar(char* character);
char* wchar_string_to_utf8_string(uint* characters);
uint* utf8_string_to_wchar_string(char* characters);

uint utf8_len_char(char* character);
#endif
