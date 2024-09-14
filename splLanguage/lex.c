#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "lex.h"

enum TokenState
{
    START, INID, ININT, INREAL, INSTRING, INCOMMENT
};

struct LexItem* getNextToken(FILE* input, int* line)
{
    char *lexeme = malloc(sizeof(char *)), current, peek;
    struct LexItem *lexP = malloc(sizeof(struct LexItem *));
    size_t length = 0;
    enum TokenState lexstate = START;

    while ((current = getc(input)))
    {       
        switch (lexstate)
        {
            case START:
                if (current == '\n')
                    (*line)++;

                if (isspace(current))
                    continue;

                if (current == EOF)
                {
                    *lexP = (struct LexItem) {lexeme, *line, DONE};
                    return lexP;
                }

                lexeme[length++] = current;

                switch(current)
                {
                    case '#':
                        lexeme[--length] = '\0';
                        lexstate = INCOMMENT;
                        break;
                    case '+':
                        *lexP = (struct LexItem) {lexeme, *line, PLUS};
                        return lexP;
                    case '-':
                        peek = getc(input);
                        lexeme[length++] = peek;
                        switch (peek)
                        {
                            case 'l': 
                                *lexP = (struct LexItem) {lexeme, *line, SLTHAN};
                                return lexP;
                            case 'g':
                                *lexP = (struct LexItem) {lexeme, *line, SGTHAN};
                                return lexP;    
                            case 'e':   
                                *lexP = (struct LexItem) {lexeme, *line, SEQ};
                                return lexP;
                            default:
                                lexeme[--length] = '\0';
                                ungetc(peek, input);
                                *lexP = (struct LexItem) {lexeme, *line, MINUS};
                                return lexP;
                        }
                    case '*':
                        peek = getc(input);
                        if (peek == '*')
                        {
                            lexeme[length++] = peek;
                            *lexP = (struct LexItem) {lexeme, *line, SREPEAT};
                            return lexP;
                        }
                        ungetc(peek, input);
                        *lexP = (struct LexItem) {lexeme, *line, MULT};
                        return lexP;
                    case '/':
                        *lexP = (struct LexItem) {lexeme, *line, DIV};
                        return lexP;
                    case '^':
                        *lexP = (struct LexItem) {lexeme, *line, EXPONENT};
                        return lexP;
                    case '=':
                        peek = getc(input);
                        if (peek == '=')
                        {
                            lexeme[length++] = peek;
                            *lexP = (struct LexItem) {lexeme, *line, NEQ};
                            return lexP;
                        }
                        ungetc(peek, input);
                        *lexP = (struct LexItem) {lexeme, *line, ASSOP};
                        return lexP;   
                    case '(':
                        *lexP = (struct LexItem) {lexeme, *line, LPAREN};
                        return lexP;
                    case ')':
                        *lexP = (struct LexItem) {lexeme, *line, RPAREN};
                        return lexP;
                    case '{':
                        *lexP = (struct LexItem) {lexeme, *line, LBRACES};
                        return lexP;                                  
                    case '}':
                        *lexP = (struct LexItem) {lexeme, *line, RBRACES};
                        return lexP;
                    case '>':
                        *lexP = (struct LexItem) {lexeme, *line, NGTHAN};
                        return lexP;  
                    case '<':
                        *lexP = (struct LexItem) {lexeme, *line, NLTHAN};
                        return lexP; 
                    case '.':
                        *lexP = (struct LexItem) {lexeme, *line, CAT};
                        return lexP;
                    case ';':
                        *lexP = (struct LexItem) {lexeme, *line, SEMICOL};
                        return lexP;
                    case ',':
                        *lexP = (struct LexItem) {lexeme, *line, COMMA};
                        return lexP;
                    case '\'':
                        lexstate = INSTRING;
                        break;
                    default:
                        if (isdigit(current))
                        {
                            lexstate = ININT;
                            break;
                        }
                        else if (isalpha(current) || current == '_' 
                        || current == '$' || current == '@')
                        {
                            lexstate = INID;
                            break;
                        }
                        else
                        {
                            printf("Lexer Error: Unrecognized Token in Line %d (%s)\n", *line, lexeme);
                        }
                        *lexP = (struct LexItem) {lexeme, *line, ERR};
                        return lexP;
                }
                break;
            case INID:
                if (!isalnum(current) && current != '_')
                {
                    ungetc(current, input);
                    if (strcmp(lexeme, "if") == 0)
                    {
                        *lexP = (struct LexItem) {lexeme, *line, IF};
                        return lexP;
                    }
                    else if (strcmp(lexeme, "else") == 0)
                    {
                        *lexP = (struct LexItem) {lexeme, *line, ELSE};
                        return lexP;
                    }
                    else if (strcmp(lexeme, "writeln") == 0)
                    {
                        *lexP = (struct LexItem) {lexeme, *line, WRITELN};
                        return lexP;
                    }
                    else
                    {
                        switch (*lexeme)
                        {
                            case '$':
                                *lexP = (struct LexItem) {lexeme, *line, NIDENT};
                                return lexP;
                            case '@':
                                *lexP = (struct LexItem) {lexeme, *line, SIDENT};
                                return lexP;
                            default:
                                *lexP = (struct LexItem) {lexeme, *line, IDENT};
                                return lexP;
                        }
                    }
                }
                lexeme[length++] = current;
                break;
            case ININT:
                lexeme[length++] = current;
                if (current == '.')
                {
                    lexstate = INREAL;
                }
                else if (!isdigit(current))
                {
                    lexeme[--length] = '\0';
                    ungetc(current, input);
                    *lexP = (struct LexItem) {lexeme, *line, ICONST};
                    return lexP;
                }
                break;
            case INREAL:
                lexeme[length++] = current;
                if (current == '.')
                {
                    printf("Lexer Error: Extra . at Line %d (%s)\n", *line, lexeme);
                    *lexP = (struct LexItem) {lexeme, *line, ERR};
                    return lexP;
                }
                if (!isdigit(current))
                {
                    ungetc(current, input);
                    lexeme[--length] = '\0';
                    *lexP = (struct LexItem) {lexeme, *line, RCONST};
                    return lexP;
                }
                break;
            case INSTRING:
                if (current == '\'')
                {
                    *lexP = (struct LexItem) {strtok(lexeme, "\'"), *line, SCONST};
                    return lexP;                    
                }
                if (current == '\n' || current == EOF)
                {
                    printf("Lexer Error: Missing Closing \' at Line %d (%s)\n", *line, lexeme);
                    *lexP = (struct LexItem) {strtok(lexeme, "\'"), *line, ERR};
                    return lexP;
                }
                lexeme[length++] = current;
                break;
            case INCOMMENT:
                if (current == '\n' || current == EOF)
                {
                    ungetc(current, input);
                    lexstate = START;
                }    
        }
    }
    return lexP;
}