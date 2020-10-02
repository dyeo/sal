#include "lexer.h"
#include "token.h"
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
	
	//for(int i=0;i<state->token_count;++i)
	//{
	//	token tok = state->tokens[i];
	//	if(tok.value == TK_STR_LITERAL)
	//	{
	//		log_debug("string (%i) : %s", tok.value, tok.string);
	//	}
	//	else if(tok.value == TK_NUM_LITERAL)
	//	{
	//		log_debug("number (%i) : %f", tok.value, tok.number);
	//	}
	//	else if(tok.value == TK_BOOL_LITERAL)
	//	{
	//		log_debug("boolean (%i) : %i", tok.value, tok.boolean);
	//	}
	//	else if(tok.value == TK_SYMBOL)
	//	{
	//		log_debug("symbol (%i) : %s", tok.value, tok.string);
	//	}
	//	else if(tok.value >= TK_KEYWORDS_START)
	//	{
	//		log_debug("keyword (%i) : %s", tok.value, keywords[tok.value-TK_KEYWORDS_START]);
	//	}
	//	else if(tok.value >= TK_TYPENAMES_START)
	//	{
	//		log_debug("typename (%i) : %s", tok.value, typenames[tok.value-TK_TYPENAMES_START]);
	//	}
	//	else
	//	{
	//		log_debug("operator (%i) : %s", tok.value, operators[tok.value-TK_OPERATORS_START]);
	//	}
	//}
	
	if(state != NULL)
	{
		free_lex_state(state);
	}
	
	return 0;
}