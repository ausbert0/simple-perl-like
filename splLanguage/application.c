#include "lex.h"
#include "parse.h"
#include "interpreter.h"
#include <stdlib.h>

struct LexItem* getNextToken(FILE*, int*);

int main(int argc, char *argv[])
{
    int lineNumber = 1;
    FILE *file;
    
    //check command-line arguments
    switch (argc)
    {
        case 1:
            printf("Missing File Name\n");
            return 1; 
        case 2:
            if ((file = fopen(*++argv, "r")) == NULL)
            {
                printf("Cannot open %s\n", *argv);
                return 1;
            }
            break;
        default:
            printf("Too many arguments\n");
            return 1;           
    }

    // parser testing
    struct ParseTree *yeet = Prog(file, &lineNumber);
    if (!yeet)
        return 0;

    // interpreter testing
    if(Interpret(yeet))
        return 1;
    return 0;

    
    // // lexical analyzer testing loop
    // struct tokMap tokenMap[] = 
    // {
    //     {WRITELN, "WRITELN"},
    //     {IF, "IF"},
    //     {ELSE, "ELSE"},
    //     {IDENT, "IDENT"},
    //     {NIDENT, "NIDENT"},
    //     {SIDENT, "SIDENT"},
    //     {ICONST, "ICONST"},
    //     {RCONST, "RCONST"},
    //     {SCONST, "SCONST"},
    //     {PLUS, "PLUS"},
    //     {MINUS, "MINUS"},
    //     {MULT, "MULT"},  
    //     {DIV, "DIV"},
    //     {EXPONENT, "EXPONENT"},
    //     {ASSOP, "ASSOP"},
    //     {NEQ, "NEQ"},
    //     {NGTHAN, "NGTHAN"},
    //     {NLTHAN, "NLTHAN"},
    //     {CAT, "CAT"},
    //     {SREPEAT, "SREPEAT"},
    //     {SEQ, "SEQ"},
    //     {SLTHAN, "SLTHAN"},
    //     {SGTHAN, "SGTHAN"},
    //     {COMMA, "COMMA"},
    //     {SEMICOL, "SEMICOL"},
    //     {LPAREN, "LPAREN"},
    //     {RPAREN, "RPAREN"},
    //     {LBRACES, "LBRACES"},
    //     {RBRACES, "RBRACES"},
    //     {ERR, "ERR"},
    //     {DONE, "DONE"}	
    // };
    
    // struct LexItem *lex = NULL;
    // while ((lex = getNextToken(file, &lineNumber)) && lex->token != ERR && lex->token != DONE)
    // {
    //     printf("%s(%s) %d\n", tokenMap[lex->token].s, lex->lexeme, lex->line);
    // }
    // printf("----------------------------------------\n");
    // return 1;

}