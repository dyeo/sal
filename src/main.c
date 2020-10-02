#include "lexer.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	
	if(argc < 2) 
	{
		log_fatal("no entry file specified\n");
		return 1;
	}
	
	lex_state *state = lexize_from_file(argv[1]);
	
	for(int i=0;i<state->token_count;++i)
	{
		log_debug("%i", state->tokens[i].value);
	}
	
	if(state != NULL)
	{
		free_lex_state(state);
	}
	
	return 0;
}