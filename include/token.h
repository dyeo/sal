#ifndef _SAL_TOKEN_H_
#define _SAL_TOKEN_H_
#ifdef __cplusplus
extern "C" {
#endif

// eof
#define TK_EOF    0
// assignment operator
#define TK_ASSIGN 1   // :
// binary operators
#define TK_DOT    2   // .
#define TK_PIPE   3   // ->
#define TK_MULT   4   // *
#define TK_DIV    5   // /
#define TK_MOD    6   // %
#define TK_ADD    7   // +
#define TK_SUB    8   // -    
#define TK_LSHFT  9   // <<
#define TK_RSHFT  10  // >>
#define TK_LESS   11  // <
#define TK_LESSEQ 12  // <
#define TK_GRTR   13  // >
#define TK_GRTREQ 14  // >
#define TK_EQ     15  // 
#define TK_NOTEQ  16  // !
#define TK_AND    17  // &
#define TK_OR     18  // |
#define TK_XOR    19  // ^
#define TK_LOGAND 20  // and
#define TK_LOGOR  21  // or
#define TK_LOGXOR 22  // xor
// unary operators
#define TK_NEG    23  // -
#define TK_NOT    24  // ~
#define TK_LOGNOT 25  // not
// other
#define TK_LBRACE 26  // {
#define TK_RBRACE 27  // }
#define TK_LPAREN 28  // (
#define TK_RPAREN 29  // )
// keywords
#define TK_SYMBOL 256
#define TK_NONE   257
#define TK_BOOL   258
#define TK_NUM    259
#define TK_STR    260
#define TK_FROM   261
#define TK_TAKE   262
#define TK_FUNC   263
#define TK_MUT    264
#define TK_RET    265
#define TK_BRK    266
#define TK_CONT   267
#define TK_FOR    268
#define TK_IN     269
#define TK_WHILE  270
#define TK_DO     271
#define TK_IF     272
#define TK_ELIF   273
#define TK_ELSE   274

typedef struct __token
{
	unsigned short value;
	unsigned int line;
	unsigned int col_start, col_end;
	char* data;
} token;

token* make_token(unsigned short value, unsigned int line, unsigned int col_start, unsigned int col_end, char* data);

#ifdef __cplusplus
}
#endif
#endif//_SAL_TOKEN_H_

