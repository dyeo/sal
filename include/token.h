#ifndef _SAL_TOKEN_H_
#define _SAL_TOKEN_H_
#ifdef __cplusplus
extern "C" {
#endif

// assignment operator
#define TK_ASSIGNOP_START 	0
#define TK_ASSIGN			TK_ASSIGNOP_START+0		// :
// binary operators
#define TK_BINARYOP_START 	1
#define TK_DOT				TK_BINARYOP_START+0		// .
#define TK_PIPE				TK_BINARYOP_START+1		// ->
#define TK_MULT				TK_BINARYOP_START+2		// *
#define TK_DIV				TK_BINARYOP_START+3		// /
#define TK_MOD				TK_BINARYOP_START+4		// %
#define TK_ADD				TK_BINARYOP_START+5		// +
#define TK_SUB				TK_BINARYOP_START+6		// -    
#define TK_LSHFT			TK_BINARYOP_START+7		// <<
#define TK_RSHFT			TK_BINARYOP_START+8		// >>
#define TK_LESS				TK_BINARYOP_START+9		// <
#define TK_LESSEQ			TK_BINARYOP_START+10	// <=
#define TK_GRTR				TK_BINARYOP_START+11	// >
#define TK_GRTREQ			TK_BINARYOP_START+12	// >=
#define TK_EQ				TK_BINARYOP_START+13	// =
#define TK_NOTEQ			TK_BINARYOP_START+14	// !=
#define TK_AND				TK_BINARYOP_START+15	// &
#define TK_OR				TK_BINARYOP_START+16	// |
#define TK_XOR				TK_BINARYOP_START+17	// ^
#define TK_LOGAND			TK_BINARYOP_START+18	// and
#define TK_LOGOR			TK_BINARYOP_START+19	// or
#define TK_LOGXOR			TK_BINARYOP_START+20	// xor
// unary operators
#define TK_UNARYOP_START 	23
#define TK_NEG				TK_UNARYOP_START+0		// -
#define TK_NOT				TK_UNARYOP_START+1		// ~
#define TK_LOGNOT			TK_UNARYOP_START+2		// not
// other
#define TK_OTHER_START 		26
#define TK_LBRACE			TK_OTHER_START+0		// {
#define TK_RBRACE			TK_OTHER_START+1		// }
#define TK_LPAREN			TK_OTHER_START+2		// (
#define TK_RPAREN			TK_OTHER_START+3		// )
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

