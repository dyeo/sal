#include "token.h"

#include <stdlib.h>

token* make_token(unsigned short value, unsigned int line, unsigned int col_start, unsigned int col_end, char* data)
{
	token* result = (token*)malloc(sizeof(token));
	result->value = value;
	result->line = line;
	result->col_start = col_start;
	result->col_end = col_end;
	result->data = data;
	return result;
}