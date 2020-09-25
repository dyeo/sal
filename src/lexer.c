#include "log.h"
#include "lexer.h"
#include "strutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

lex_state* make_lex_state(char* buffer)
{
	lex_state* r = (lex_state*)malloc(sizeof(lex_state));
	{		
		r->status = LEXER_IN_PROGRESS;
		r->buffer = buffer;
		r->buffer_end = buffer + strlen(buffer);
		r->total_lines = count_chars(buffer, '\n');
		r->curr = buffer;
		r->line = 0;
		r->tokens = 0;
		r->token_count = 0;
	}
	return r;
}

void free_lex_state(lex_state* state)
{
	free(state->buffer);
	free(state);
}

void parse_next_token(lex_state *state);

//
//
//
lex_state *lexize_from_file(const char *filename)
{
	log_debug(filename, -1, "lexing file...");
	
	FILE    *infile;
	char    *buffer;
	long    numbytes;
	if((infile = fopen(filename, "r")) == NULL)
	{
		log_fatal(filename, -1, "file does not exist");
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	numbytes = ftell(infile);
	fseek(infile, 0L, SEEK_SET);
	buffer = (char*)calloc(numbytes, sizeof(char));
	if(buffer == NULL)
	{
		log_fatal(filename, -1, "memory error when reading file");
		return NULL;
	}
	
	/* copy all the text into the buffer */
	fread(buffer, sizeof(char), numbytes, infile);
	fclose(infile);
	
	lex_state* state = make_lex_state(buffer);
	
	while(state->status == LEXER_IN_PROGRESS)
	{
		parse_next_token(state);
	}
	
	if(state->status == LEXER_SUCCESS)
	{
		log_debug(filename, state->total_lines, "lexing complete");
		return state;
	}
	
	log_fatal(filename, -1, "lexing failed - see above errors");
	free_lex_state(state);
	return NULL;
}

char peek_curr(lex_state *s)
{
	return *(s->curr);
}

char peek_next(lex_state *s)
{
	return *(s->curr+1);
}

//
//
//
void parse_next_token(lex_state *s)
{
	char *last;
	char token;
	
	while(token = *s->curr)
	{
		++s->curr;
		if(token == '\r' && *(s->curr) == '\n')
		{
			s->line += 1;
			s->curr += 1;
		}
		else if (token == '\r' || token == '\n')
		{
			s->line += 1;
		}
		else
		{
			printf("%c", token);
		}
	}
	
	if(s->curr >= s->buffer_end)
	{
		s->status = LEXER_SUCCESS;
	}
}
