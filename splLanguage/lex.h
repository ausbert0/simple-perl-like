#ifndef LEX_H_
#define LEX_H_
#include <stdio.h>
//Definition of all the possible token types
enum Token {
	// keywords
	WRITELN, IF, ELSE, 

	// identifiers
	IDENT, NIDENT, SIDENT,

	// an integer, real, and string constant
	ICONST, RCONST, SCONST, 

	// the numeric operators, assignment, numeric and string comparison operators
	PLUS, MINUS, MULT, DIV, EXPONENT, ASSOP, NEQ, 
	NGTHAN, NLTHAN, CAT, SREPEAT, SEQ, SLTHAN, SGTHAN, 

	//Delimiters
	COMMA, SEMICOL, LPAREN, RPAREN, LBRACES, RBRACES, 
	
    // any error returns this token
	ERR,

	// when completed (EOF), return this token
	DONE,
};

// solely for printing token types for testing
struct tokMap 
{
	enum Token t;
	char *s;
};

struct LexItem 
{
    char *lexeme;
	int line;
	enum Token token;
};

extern struct LexItem* getNextToken(FILE* input, int* line);

#endif /* LEX_H_ */