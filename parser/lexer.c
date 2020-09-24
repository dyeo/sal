#include "lexer.h"

#include <stdlib.h>
#include <stdio.h>

lex_state* make_lex_state(char* buffer)
{
	lex_state* result = (lex_state*)malloc(sizeof(lex_state));
	result->buffer = buffer;
	return result;
}

void free_lex_state(lex_state* state)
{
	free(state->buffer);
	free(state);
}

token** tokenize(lex_state* state)
{
	printf("%s", state->buffer);
	return NULL;
}