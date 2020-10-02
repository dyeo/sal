#ifndef _SAL_LEXER_H_
#define _SAL_LEXER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "token.h"

#define LEXER_IN_PROGRESS 0
#define LEXER_SUCCESS     1
#define LEXER_ERROR       2

typedef struct __lex_state
{
	const char *filename;
	unsigned char *buffer; // the file buffer
	unsigned char *buffer_end; // the total length of the buffer
	int total_lines; // the line count of the buffer
	unsigned char *curr; // current position in buffer
	int col; // current column
	int line; // the current line
	token *tokens; // the tokens array
	int token_capacity;
	int token_count; // the number of tokens found
} 
lex_state;

lex_state *make_lex_state(const unsigned char *filename, unsigned char *buffer);

void free_lex_state(lex_state *state);

lex_state *lexize_from_file(const unsigned char *filename);

#ifdef __cplusplus
}
#endif
#endif//_SAL_LEXER_H_