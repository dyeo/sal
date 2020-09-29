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
		r->buffer = buffer;
		r->buffer_end = buffer + strlen(buffer);
		r->total_lines = count_chars(buffer, '\n') + 1;
		r->curr = buffer;
		r->col = 1;
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

int parse_next_token(lex_state *state);

//
//
//
lex_state *lexize_from_file(const char *filename)
{	
	log_debug("lexing file...");
	
	FILE *infile;
	char *buffer;
	long numbytes;
	if((infile = fopen(filename, "r")) == NULL)
	{
		log_fatal("file does not exist");
		return NULL;
	}
	fseek(infile, 0L, SEEK_END);
	numbytes = ftell(infile);
	fseek(infile, 0L, SEEK_SET);
	buffer = (char*)calloc(numbytes, sizeof(char));
	if(buffer == NULL)
	{
		log_fatal("memory error when reading file");
		return NULL;
	}
	
	/* copy all the text into the buffer */
	fread(buffer, sizeof(char), numbytes, infile);
	fclose(infile);
	
	lex_state* state = make_lex_state(filename, buffer);
	
	int status;
	while((status = parse_next_token(state)) == LEXER_IN_PROGRESS)
	{ }
	
	log_debug("length: %i", strlen(state->buffer));
	log_debug("index: %i", state->curr-state->buffer);

	if(status == LEXER_SUCCESS)
	{
		log_debug("lexing complete");
		return state;
	}
	
	log_fatal("lexing failed - see above errors");
	free_lex_state(state);
	return NULL;
}

bool skip_newline(lex_state *s)
{
	if(*(s->curr) == '\r' && *(s->curr+1) == '\n')
	{
		++s->line;
		s->curr+=2;
		s->col = 1;
		return true;
	}
	else if (*(s->curr) == '\r' || *(s->curr) == '\n')
	{
		++s->line;
		s->curr+=1;
		s->col = 1;
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
	return c == '_' || (c >= '@' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool is_ident_char(char c)
{
	return c == '_' || (c >= '0' && c <= '9') || (c >= '@' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool is_hex_num(char c)
{
	return (c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'F')
		|| (c >= 'a' && c <= 'f');
}

bool is_bin_num(char c)
{
	return (c >= '0' && c <= '1')
		|| (c == '_');
}

// TODO: keep track of number of decimals: throw error if more than 1
bool is_dec_num(char c)
{
	return (c >= '0' && c <= '9')
		|| (c == '.');
}

//
//
//
int parse_next_token(lex_state *s)
{
	// ignore new lines
	if(skip_newline(s))
	{
		//printf("%c", *s->curr);
		return LEXER_IN_PROGRESS;
	}
	else if(isspace(*s->curr))
	{
		++s->curr;
		++s->col;
		return LEXER_IN_PROGRESS;
	}
	
	// ignore comments up until newline
	if(*s->curr == ';')
	{
		while(!skip_newline(s))
		{
			++s->curr;
			++s->col;
		}
		++s->curr;
		++s->col;
		return LEXER_IN_PROGRESS;
	}
		// beginning of string found
	if(*s->curr == '"')
	{
		// keep scanning until the end of the string (skipping escape sequences)
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
		logf_debug(s->filename, s->line, s->col, "  STRING\t%i\t(%s)", TK_STR_LITERAL, string); // TEMPORARY
		free(string); // TEMPORARY
		s->curr = temp;
		s->col += (temp-s->curr);
		return LEXER_IN_PROGRESS;
	}
		// beginning of identifier found
	if(is_ident_start(*s->curr))
	{
		char *temp = s->curr;
		// keep scanning until the end of the identifier
		do { ++temp; }
		while(is_ident_char(*temp));
		// copy identifier out of buffer to store later
		char *ident = substr(s->curr, temp);
		bool is_symbol = true;
		// determine if this is a bool literal
		if(strcmp(ident,"true") == 0 || strcmp(ident,"false") == 0)
		{
			is_symbol = false;
			logf_debug(s->filename, s->line, s->col, "BOOLEAN\t%i\t(%s)", TK_BOOL_LITERAL, ident); // TEMPORARY
		}				
		// determine if this is a typename
		for(int i=0;i<NUM_TYPENAMES;++i)
		{
			if(strcmp(ident,typenames[i]) == 0)
			{
				is_symbol = false;
				logf_debug(s->filename, s->line, s->col, "TYPENAME\t%i\t(%s)", TK_TYPENAMES_START+i, ident); // TEMPORARY
				break;
			}
		}
		// determine if this is a reserved keyword or a symbol
		for(int i=0;i<NUM_KEYWORDS;++i)
		{
			if(strcmp(ident,keywords[i]) == 0)
			{
				is_symbol = false;
				logf_debug(s->filename, s->line, s->col, " KEYWORD\t%i\t(%s)", TK_KEYWORDS_START+i, ident); // TEMPORARY
				break;
			}
		}
		// otherwise it's a symbol
		if(is_symbol)
		{
			logf_debug(s->filename, s->line, s->col, "  SYMBOL\t%i\t(%s)", TK_SYMBOL, ident); // TEMPORARY
		}			
		free(ident); // TEMPORARY
		
		s->curr = temp;
		s->col += (temp-s->curr);
		return LEXER_IN_PROGRESS;
	}
	
	// beginning of number found
	if(isdigit(*s->curr))
	{
		char *temp = s->curr;
		if(*s->curr == '0' && *(temp+1) == 'x') // it's a hexadecimal number
		{
			temp+=2;
			do { ++temp; }
			while(is_hex_num(*temp));
		}
		else if(*s->curr == '0' && *(temp+1) == 'b') // it's a binary number
		{
			temp+=2;
			do { ++temp; }
			while(is_bin_num(*temp));
		}
		else // it's a regular number
		{
			do { ++temp; }
			while(is_dec_num(*temp));
		}
		char *number = substr(s->curr, temp);
		logf_debug(s->filename, s->line, s->col, "  NUMBER\t%i\t(%s)", TK_NUM_LITERAL, number); // TEMPORARY
		free(number); // TEMPORARY
		s->curr = temp;
		s->col += (temp-s->curr);
		return LEXER_IN_PROGRESS;
	}
	
	// check if operator/delimiter
	for(int o=0;o<NUM_OPERATORS;++o)
	{
		char *temp = s->curr;
		int oplen = strlen(operators[o]);
		if(strncmp(s->curr, operators[o], oplen) == 0)
		{
			temp += oplen;
			char *operator = substr(s->curr, temp);
			logf_debug(s->filename, s->line, s->col, "OPERATOR\t%i\t(%s)", TK_OPERATORS_START+o, operator); // TEMPORARY
			free(operator); // TEMPORARY
			s->curr = temp;
			s->col += (temp-s->curr);
			return LEXER_IN_PROGRESS;
		}
	}
	
	if(s->curr >= s->buffer_end || *s->curr == '\0')
	{
		return LEXER_SUCCESS;
	}
	
	logf_error(s->filename, s->line, s->col, "unidentified token '%c'", *s->curr);
	return LEXER_ERROR;
}
