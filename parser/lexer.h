#ifndef _SAL_LEXER_H_
#define _SAL_LEXER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "token.h"

typedef struct __lex_state
{
	char* buffer;
} lex_state;

lex_state* make_lex_state(char* buffer);

void free_lex_state(lex_state* state);

token** tokenize(lex_state* state);

#ifdef __cplusplus
}
#endif
#endif//_SAL_LEXER_H_