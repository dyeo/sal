#include "lexer.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{	
	if(argc < 2) 
	{
		log_fatal("sal", -1, "no entry file specified\n");
		return 1;
	}
	
	lex_state *state = lexize_from_file(argv[1]);
	
	if(state != NULL)
	{
		free_lex_state(state);
	}
	
	return 0;
}