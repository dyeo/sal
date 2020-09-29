#include "log.h"
#include "lexer.h"
#include "strutil.h"
#include "utf8.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

lex_state* make_lex_state(const unsigned char *filename, unsigned char* buffer)
{
	lex_state* r = (lex_state*)malloc(sizeof(lex_state));
	{
		r->filename = filename;
		r->buffer = buffer;
		r->buffer_end = buffer + utf8size(buffer);
		r->total_lines = cntchr(buffer, '\n') + 1;
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
lex_state *lexize_from_file(const unsigned char *filename)
{	
	log_debug("lexing file...");
		
	FILE *infile;
	unsigned char *buffer;
	long numbytes;
	if((infile = fopen(filename, "r")) == NULL)
	{
		log_fatal("file does not exist");
		return NULL;
	}
	fseek(infile, 0L, SEEK_END);
	numbytes = ftell(infile);
	fseek(infile, 0L, SEEK_SET);
	buffer = (unsigned char*)calloc(numbytes, sizeof(unsigned char));
	if(buffer == NULL)
	{
		log_fatal("memory error when reading file");
		return NULL;
	}
	
	/* copy all the text into the buffer */
	fread(buffer, sizeof(unsigned char), numbytes, infile);
	fclose(infile);
	
	lex_state* state = make_lex_state(filename, buffer);
	
	int status;
	while((status = parse_next_token(state)) == LEXER_IN_PROGRESS)
	{ }

	if(status == LEXER_SUCCESS)
	{
		log_debug("lexing complete");
		return state;
	}
	
	log_fatal("lexing failed - see above errors");
	free_lex_state(state);
	return NULL;
}

bool is_escape(unsigned char c, unsigned char n)
{
	return c == '\\' && (utf8chr("abfnrtv\\\'\"\?", n) != NULL);
}

bool is_ident_start(unsigned char c)
{
	return c == '_' || (c >= '@' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 128);
}

bool is_ident_char(unsigned char c)
{
	return c == '_' || (c >= '0' && c <= '9') || (c >= '@' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 128);
}

bool is_hex_num(unsigned char c)
{
	return (c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'F')
		|| (c >= 'a' && c <= 'f');
}

bool is_bin_num(unsigned char c)
{
	return (c >= '0' && c <= '1')
		|| (c == '_');
}

// TODO: keep track of number of decimals: throw error if more than 1
bool is_dec_num(unsigned char c)
{
	return (c >= '0' && c <= '9')
		|| (c == '.');
}

//
//
//
int parse_next_token(lex_state *s)
{
	// windows newline
	if(*(s->curr) == '\r' && *(s->curr+1) == '\n')
	{
		++s->line;
		s->curr+=2;
		s->col = 1;
		return LEXER_IN_PROGRESS;
	}
	// other newlines
	if (*(s->curr) == '\r' || *(s->curr) == '\n')
	{
		printf("%c", '\n');
		++s->line;
		s->curr+=1;
		s->col = 1;
		return LEXER_IN_PROGRESS;
	}
	// ignore whitespace
	if(isspace(*s->curr))
	{
		++s->curr;
		++s->col;
		//logf_debug(s->filename, s->line, s->col, "\e[90mSPACE\e[0m");
		return LEXER_IN_PROGRESS;
	}	
	// ignore comments up until newline
	if(*s->curr == ';')
	{
		unsigned char *temp = s->curr;
		while(*temp != '\r' && *temp != '\n')
		{ ++temp; }		
		s->col += (temp-s->curr);
		s->curr = temp;
		return LEXER_IN_PROGRESS;
	}
	// beginning of string found
	if(*s->curr == '"')
	{
		// keep scanning until the end of the string (skipping escape sequences)
		unsigned char *temp = s->curr;
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
		unsigned char *string = substr(s->curr+1, temp-1);
		s->col += (temp-s->curr);
		s->curr = temp;
		logf_debug(s->filename, s->line, s->col, "  STRING\t%i\t(%s)", TK_STR_LITERAL, string); // TEMPORARY
		free(string); // TEMPORARY
		return LEXER_IN_PROGRESS;
	}
	// beginning of identifier found
	if(is_ident_start(*s->curr))
	{
		unsigned char *temp = s->curr;
		// keep scanning until the end of the identifier
		do
		{
			++temp;
		}
		while(is_ident_char(*temp));
		// copy identifier out of buffer to store later
		unsigned char *ident = substr(s->curr, temp);
		s->col += (temp-s->curr);
		s->curr = temp;
		bool is_symbol = true;
		// determine if this is a bool literal
		if(utf8cmp(ident,"true") == 0 || strcmp(ident,"false") == 0)
		{
			is_symbol = false;
			logf_debug(s->filename, s->line, s->col, "BOOLEAN\t%i\t(%s)", TK_BOOL_LITERAL, ident); // TEMPORARY
		}				
		// determine if this is a typename
		for(int i=0;i<NUM_TYPENAMES;++i)
		{
			if(utf8cmp(ident,typenames[i]) == 0)
			{
				is_symbol = false;
				logf_debug(s->filename, s->line, s->col, "TYPENAME\t%i\t(%s)", TK_TYPENAMES_START+i, ident); // TEMPORARY
				break;
			}
		}
		// determine if this is a reserved keyword or a symbol
		for(int i=0;i<NUM_KEYWORDS;++i)
		{
			if(utf8cmp(ident,keywords[i]) == 0)
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
		return LEXER_IN_PROGRESS;
	}
	// beginning of number found
	if(isdigit(*s->curr))
	{
		unsigned char *temp = s->curr;
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
		unsigned char *number = substr(s->curr, temp);
		s->col += (temp-s->curr);
		s->curr = temp;
		logf_debug(s->filename, s->line, s->col, "  NUMBER\t%i\t(%s)", TK_NUM_LITERAL, number); // TEMPORARY
		free(number); // TEMPORARY
		return LEXER_IN_PROGRESS;
	}
	// check if operator/delimiter
	for(int o=0;o<NUM_OPERATORS;++o)
	{
		unsigned char *temp = s->curr;
		int oplen = utf8len(operators[o]);
		if(utf8ncmp(s->curr, operators[o], oplen) == 0)
		{
			temp += oplen;
			unsigned char *operator = substr(s->curr, temp);
			s->col += (temp-s->curr);
			s->curr = temp;
			logf_debug(s->filename, s->line, s->col, "OPERATOR\t%i\t(%s)", TK_OPERATORS_START+o, operator); // TEMPORARY
			free(operator); // TEMPORARY
			return LEXER_IN_PROGRESS;
		}
	}
	// end of string (file) found
	if(s->curr >= s->buffer_end || *s->curr == '\0')
	{
		return LEXER_SUCCESS;
	}
	// unexpected token (fall-through)
	logf_error(s->filename, s->line, s->col, "unidentified token '%c'", *s->curr);
	return LEXER_ERROR;
}
