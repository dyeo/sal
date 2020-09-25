#include "log.h"
#include "lexer.h"
#include "strutil.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

lex_state* make_lex_state(const char *filename, char* buffer)
{
	lex_state* r = (lex_state*)malloc(sizeof(lex_state));
	{
		r->filename = filename;
		r->status = LEXER_IN_PROGRESS;
		r->buffer = buffer;
		r->buffer_end = buffer + strlen(buffer);
		r->total_lines = count_chars(buffer, '\n') + 1;
		r->curr = buffer;
		r->line = 1;
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
	log_debug(filename, 0, "lexing file...");
	
	FILE    *infile;
	char    *buffer;
	long    numbytes;
	if((infile = fopen(filename, "r")) == NULL)
	{
		log_fatal(filename, 0, "file does not exist");
		return NULL;
	}
	fseek(infile, 0, SEEK_END);
	numbytes = ftell(infile);
	fseek(infile, 0L, SEEK_SET);
	buffer = (char*)calloc(numbytes, sizeof(char));
	if(buffer == NULL)
	{
		log_fatal(filename, 0, "memory error when reading file");
		return NULL;
	}
	
	/* copy all the text into the buffer */
	fread(buffer, sizeof(char), numbytes, infile);
	fclose(infile);
	
	lex_state* state = make_lex_state(filename, buffer);
	
	while(state->status == LEXER_IN_PROGRESS)
	{
		parse_next_token(state);
	}
	
	if(state->status == LEXER_SUCCESS)
	{
		log_debug(filename, state->total_lines, "lexing complete");
		return state;
	}
	
	log_fatal(filename, 0, "lexing failed - see above errors");
	free_lex_state(state);
	return NULL;
}

bool is_ident_start(char c)
{
	return c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool is_ident_char(char c)
{
	return c == '_' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

//
//
//
void parse_next_token(lex_state *s)
{
	char token;
	
	while(token = *s->curr)
	{
		// ignore new lines
		if(token == '\r' && *(s->curr+1) == '\n')
		{
			++s->line;
			++s->curr;
		}
		else if (token == '\r' || token == '\n')
		{
			++s->line;
		}
		
		// ignore comments up until newline
		if(token == ';')
		{
			while(token = *s->curr)
			{
				if(token == '\r' && *(s->curr+1) == '\n')
				{
					++s->curr;
					break;
				}
				else if (token == '\r' || token == '\n')
				{
					++s->line;
					break;
				}
				++s->curr;
			}
			++s->line;
		}
	
		// beginning of identifier found
		if(is_ident_start(token))
		{
			// begin scanning until the end of the identifier
			char *temp = s->curr;
			do
			{
				++temp;
			}
			while(is_ident_char(*temp));
			// copy identifier out of buffer to store later
			char *ident = substr(s->curr, temp);
			log_debug(s->filename, s->line, "identifier found: %s", ident);
			free(ident);
			
			s->curr = temp;
		}
		//	if curr:temp in keywords:
		//		make keyword token from ident
		//	else
		//		make symbol token from ident
		
		++s->curr;
	}
	
	if(s->curr >= s->buffer_end)
	{
		s->status = LEXER_SUCCESS;
	}
}
