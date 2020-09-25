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

bool skip_newline(lex_state *s)
{
	if(*(s->curr) == '\r' && *(s->curr+1) == '\n')
	{
		++s->line;
		++s->curr;
		return true;
	}
	else if (*(s->curr) == '\r' || *(s->curr) == '\n')
	{
		++s->line;
		return true;
	}
	return false;
}

bool is_escape(char c, char n)
{
	return c == '\\' && (strchr("abfnrtv\\\'\"\?", n) != NULL);
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
		if(skip_newline(s))
		{
			++s->curr;
			return;
		}
		
		// ignore comments up until newline
		if(token == ';')
		{
			while(!skip_newline(s))
			{
				++s->curr;
			}
			++s->curr;
			return;
		}
	
		// beginning of string found
		if(token == '"')
		{
			char *temp = s->curr;
			do
			{
				++temp;
				if(is_escape(*temp, *(temp+1)))
				{
					temp+=2;
				}
			}		
			while(*temp != '"');
			++temp;
			char *string = substr(s->curr+1, temp-1);
			log_debug(s->filename, s->line, "STRING:\t%s", string);
			free(string);
			s->curr = temp;
			return;
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
			// determine if this is a reserved keyword
			bool is_symbol = true;
			for(int i=0;i<NUM_KEYWORDS;++i)
			{
				if(strcmp(ident,keywords[i]) == 0)
				{
					log_debug(s->filename, s->line, "KYWORD:\t%i\t(%s)", TK_KEYWORDS_START+i, ident);
					is_symbol = false;
					break;
				}
			}
			
			if(is_symbol)
			{
				log_debug(s->filename, s->line, "SYMBOL:\t%i\t(%s)", TK_SYMBOL, ident);
			}
			
			free(ident);
			
			s->curr = temp;
			return;
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
