#ifndef _SAL_TOKEN_H_
#define _SAL_TOKEN_H_
#ifdef __cplusplus
extern "C" {
#endif

// assignment operator
#define TK_ASSIGN	0	// :
#define TK_DOT		1	// .
#define TK_PIPE		2	// ->
#define TK_MULT		3	// *
#define TK_DIV		4	// /
#define TK_MOD		5	// %
#define TK_ADD		6	// +
#define TK_SUB		7	// -    
#define TK_LSHFT	8	// <<
#define TK_RSHFT	9	// >>
#define TK_LESS		10	// <
#define TK_LESSEQ	11	// <=
#define TK_GRTR		12	// >
#define TK_GRTREQ	13	// >=
#define TK_EQ		14	// =
#define TK_NOTEQ	15	// !=
#define TK_AND		16	// &
#define TK_OR		17	// |
#define TK_XOR		18	// ^
#define TK_LOGAND	19	// and
#define TK_LOGOR	20	// or
#define TK_LOGXOR	21	// xor
// unary operators
#define TK_NEG		23	// -
#define TK_NOT		24	// ~
#define TK_LOGNOT	25	// not
// other
#define TK_LBRACE	26	// {
#define TK_RBRACE	27	// }
#define TK_LPAREN	28	// (
#define TK_RPAREN	29	// )
// keywords
#define TK_NONE		128
#define TK_BOOL		129
#define TK_NUM		130
#define TK_STR		131
#define TK_FROM		132
#define TK_TAKE		133
#define TK_MUT		134
#define TK_FUNC		135
#define TK_RET		136
#define TK_BRK		137
#define TK_CONT		138
#define TK_FOR		139
#define TK_IN		140
#define TK_WHILE	141
#define TK_DO		142
#define TK_IF		143
#define TK_ELIF		144
#define TK_ELSE		145
// symbol
#define TK_SYMBOL	256

#define NUM_OPERATORS 29
static const char *operators[] = {
	":", 
	".", "->",
	"*", "/", "%",
	"+", "-",
	"<<", ">>",
	"<", "<=",
	">", ">=",
	"=", "!=", 
	"&", "|", "^",
	"and", "or", "xor",
	"-", "~", "not",
	"{", "}", "(", ")"
};

#define NUM_KEYWORDS 18
static const char *keywords[] = {
	"none", "bool", "num", "str",
	"from", "take",
	"mut", "func",
	"ret", "brk", "cont",
	"for", "in",
	"while", "do",
	"if", "elif", "else"
};

typedef struct __token
{
	unsigned short value;
	unsigned int line;
	unsigned int col_start, col_end;
	char *data;
} 
token;

token *make_token(unsigned short value, unsigned int line, unsigned int col_start, unsigned int col_end, char *data);

#ifdef __cplusplus
}
#endif
#endif//_SAL_TOKEN_H_

