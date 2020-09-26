#ifndef _SAL_TOKEN_H_
#define _SAL_TOKEN_H_
#ifdef __cplusplus
extern "C" {
#endif

// assignment operator
#define TK_OPERATORS_START 	0
#define TK_ASSIGN			TK_OPERATORS_START+0		// :
#define TK_DOT				TK_OPERATORS_START+1		// .
#define TK_PIPE				TK_OPERATORS_START+2		// ->
#define TK_MULT				TK_OPERATORS_START+3		// *
#define TK_DIV				TK_OPERATORS_START+4		// /
#define TK_MOD				TK_OPERATORS_START+5		// %
#define TK_ADD				TK_OPERATORS_START+6		// +
#define TK_SUB				TK_OPERATORS_START+7		// -    
#define TK_LSHFT			TK_OPERATORS_START+8		// <<
#define TK_RSHFT			TK_OPERATORS_START+9		// >>
#define TK_LESS				TK_OPERATORS_START+10	// <
#define TK_LESSEQ			TK_OPERATORS_START+11	// <=
#define TK_GRTR				TK_OPERATORS_START+12	// >
#define TK_GRTREQ			TK_OPERATORS_START+13	// >=
#define TK_EQ				TK_OPERATORS_START+14	// =
#define TK_NOTEQ			TK_OPERATORS_START+15	// !=
#define TK_AND				TK_OPERATORS_START+16	// &
#define TK_OR				TK_OPERATORS_START+17	// |
#define TK_XOR				TK_OPERATORS_START+18	// ^
#define TK_LOGAND			TK_OPERATORS_START+19	// and
#define TK_LOGOR			TK_OPERATORS_START+20	// or
#define TK_LOGXOR			TK_OPERATORS_START+21	// xor
#define TK_NEG				TK_OPERATORS_START+22	// -
#define TK_NOT				TK_OPERATORS_START+23	// ~
#define TK_LOGNOT			TK_OPERATORS_START+24	// not
#define TK_LBRACE			TK_OPERATORS_START+25	// {
#define TK_RBRACE			TK_OPERATORS_START+26	// }
#define TK_LPAREN			TK_OPERATORS_START+27	// (
#define TK_RPAREN			TK_OPERATORS_START+28	// )
// keywords
#define TK_KEYWORDS_START 	128
#define TK_NONE				TK_KEYWORDS_START+0	
#define TK_BOOL				TK_KEYWORDS_START+1	
#define TK_NUM				TK_KEYWORDS_START+2	
#define TK_STR				TK_KEYWORDS_START+3	
#define TK_FROM				TK_KEYWORDS_START+4	
#define TK_TAKE				TK_KEYWORDS_START+5	
#define TK_MUT				TK_KEYWORDS_START+6	
#define TK_FUNC				TK_KEYWORDS_START+7	
#define TK_RET				TK_KEYWORDS_START+8	
#define TK_BRK				TK_KEYWORDS_START+9	
#define TK_CONT				TK_KEYWORDS_START+10
#define TK_FOR				TK_KEYWORDS_START+11
#define TK_IN				TK_KEYWORDS_START+12
#define TK_WHILE			TK_KEYWORDS_START+13
#define TK_DO				TK_KEYWORDS_START+14
#define TK_IF				TK_KEYWORDS_START+15
#define TK_ELIF				TK_KEYWORDS_START+16
#define TK_ELSE				TK_KEYWORDS_START+17
// special
#define TK_SPECIAL_START	256
#define TK_SYMBOL			TK_SPECIAL_START+0

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
	unsigned int col;
	char *data;
} 
token;

token *make_token(unsigned short value, unsigned int line, unsigned int col, char *data);

#ifdef __cplusplus
}
#endif
#endif//_SAL_TOKEN_H_

