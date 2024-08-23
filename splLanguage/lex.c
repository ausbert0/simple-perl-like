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
    char lexeme[100] = {'\0'}, current, peek;
    struct LexItem *lexP = malloc(sizeof(struct LexItem));
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
                    *lexP = (struct LexItem) {DONE, lexeme, *line};
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
                        *lexP = (struct LexItem) {PLUS, lexeme, *line};
                        return lexP;
                    case '-':
                        peek = getc(input);
                        lexeme[length++] = peek;
                        switch (peek)
                        {
                            case 'l': 
                                *lexP = (struct LexItem) {SLTHAN, lexeme, *line};
                                return lexP;
                            case 'g':
                                *lexP = (struct LexItem) {SGTHAN, lexeme, *line};
                                return lexP;    
                            case 'e':   
                                *lexP = (struct LexItem) {SEQ, lexeme, *line};
                                return lexP;
                            default:
                                lexeme[--length] = '\0';
                                ungetc(peek, input);
                                *lexP = (struct LexItem) {MINUS, lexeme, *line};
                                return lexP;
                        }
                    case '*':
                        peek = getc(input);
                        if (peek == '*')
                        {
                            lexeme[length++] = peek;
                            *lexP = (struct LexItem) {SREPEAT, lexeme, *line};
                            return lexP;
                        }
                        ungetc(peek, input);
                        *lexP = (struct LexItem) {MULT, lexeme, *line};
                        return lexP;
                    case '/':
                        *lexP = (struct LexItem) {DIV, lexeme, *line};
                        return lexP;
                    case '^':
                        *lexP = (struct LexItem) {EXPONENT, lexeme, *line};
                        return lexP;
                    case '=':
                        peek = getc(input);
                        if (peek == '=')
                        {
                            lexeme[length++] = peek;
                            *lexP = (struct LexItem) {NEQ, lexeme, *line};
                            return lexP;
                        }
                        ungetc(peek, input);
                        *lexP = (struct LexItem) {ASSOP, lexeme, *line};
                        return lexP;   
                    case '(':
                        *lexP = (struct LexItem) {LPAREN, lexeme, *line};
                        return lexP;
                    case ')':
                        *lexP = (struct LexItem) {RPAREN, lexeme, *line};
                        return lexP;
                    case '{':
                        *lexP = (struct LexItem) {LBRACES, lexeme, *line};
                        return lexP;                                  
                    case '}':
                        *lexP = (struct LexItem) {RBRACES, lexeme, *line};
                        return lexP;
                    case '>':
                        *lexP = (struct LexItem) {NGTHAN, lexeme, *line};
                        return lexP;  
                    case '<':
                        *lexP = (struct LexItem) {NLTHAN, lexeme, *line};
                        return lexP; 
                    case '.':
                        *lexP = (struct LexItem) {CAT, lexeme, *line};
                        return lexP;
                    case ';':
                        *lexP = (struct LexItem) {SEMICOL, lexeme, *line};
                        return lexP;
                    case ',':
                        *lexP = (struct LexItem) {COMMA, lexeme, *line};
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
                        *lexP = (struct LexItem) {ERR, lexeme, *line};
                        return lexP;
                }
                break;
            case INID:
                if (!isalnum(current) && current != '_')
                {
                    ungetc(current, input);
                    if (strcmp(lexeme, "if") == 0)
                    {
                        *lexP = (struct LexItem) {IF, lexeme, *line};
                        return lexP;
                    }
                    else if (strcmp(lexeme, "else") == 0)
                    {
                        *lexP = (struct LexItem) {ELSE, lexeme, *line};
                        return lexP;
                    }
                    else if (strcmp(lexeme, "writeln") == 0)
                    {
                        *lexP = (struct LexItem) {WRITELN, lexeme, *line};
                        return lexP;
                    }
                    else
                    {
                        switch (*lexeme)
                        {
                            case '$':
                                *lexP = (struct LexItem) {NIDENT, lexeme, *line};
                                return lexP;
                            case '@':
                                *lexP = (struct LexItem) {SIDENT, lexeme, *line};
                                return lexP;
                            default:
                                *lexP = (struct LexItem) {IDENT, lexeme, *line};
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
                    *lexP = (struct LexItem) {ICONST, lexeme, *line};
                    return lexP;
                }
                break;
            case INREAL:
                lexeme[length++] = current;
                if (current == '.')
                {
                    printf("Lexer Error: Extra . at Line %d (%s)\n", *line, lexeme);
                    *lexP = (struct LexItem) {ERR, lexeme, *line};
                    return lexP;
                }
                if (!isdigit(current))
                {
                    ungetc(current, input);
                    lexeme[--length] = '\0';
                    *lexP = (struct LexItem) {RCONST, lexeme, *line};
                    return lexP;
                }
                break;
            case INSTRING:
                if (current == '\'')
                {
                    *lexP = (struct LexItem) {SCONST, strtok(lexeme, "\'"), *line};
                    return lexP;                    
                }
                if (current == '\n' || current == EOF)
                {
                    printf("Lexer Error: Missing Closing \' at Line %d (%s)\n", *line, lexeme);
                    *lexP = (struct LexItem) {ERR, strtok(lexeme, "\'"), *line};
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