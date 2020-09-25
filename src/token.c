#include "token.h"

#include <stdlib.h>

token* make_token(unsigned short value, unsigned int line, unsigned int col, char* data)
{
	token* result = (token*)malloc(sizeof(token));
	{
		result->value = value;
		result->line = line;
		result->col = col;
		result->data = data;
	}
	return result;
}