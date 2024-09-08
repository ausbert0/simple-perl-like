#include "interpreter.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void InterpreterError(char *msg) {
    printf("Runtime Error: %s\n", msg);
}

struct Value *neq(struct Value *left, struct Value *right)
{   
    double operand1 = 0, operand2 = 0;
    char *remainingCharacters = {'\0'};
    switch (left->type)
    {
        case VREAL:
            operand1 = left->real;
            break;
        case VINT:
            operand1 = left->integer;
            break;
        case VSTRING:
            if (!(operand1 = strtod(left->string, &remainingCharacters))) {
                InterpreterError("Invalid conversion from string to double");
                return NULL;
            }
            break;
        default:
            InterpreterError("Booleans cannot be compared with numerical operators");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            operand2 = right->real;
            break;
        case VINT:
            operand2 = right->integer;
            break;
        case VSTRING:
            if (!(operand2 = strtod(left->string, &remainingCharacters))) {
                InterpreterError("Invalid conversion from string to double");
                return NULL;
            }
            break;
        default:
            InterpreterError("Booleans cannot be compared with numerical operators");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value *));
    result->bool = operand1 == operand2;
    result->type = VBOOL;
    return result;
}

struct Value *seq(struct Value *left, struct Value *right)
{
    char *operand1 = NULL, *operand2 = NULL;
    switch (left->type)
    {
        case VREAL:
            sprintf(operand1, "%f", left->real);
            break;
        case VINT:
            sprintf(operand1, "%d", left->integer);
            break;
        case VSTRING:
            operand1 = left->string;
            break;
        default:
            InterpreterError("Booleans cannot be compared with string operators");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            sprintf(operand2, "%f", right->real);
            break;
        case VINT:
            sprintf(operand2, "%d", right->integer);
            break;
        case VSTRING:
            operand2 = right->string;
            break;
        default:
            InterpreterError("Booleans cannot be compared with string operators");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value *));
    result->bool = strcmp(operand1, operand2) == 0;
    result->type = VBOOL;
    return result;
}

struct Value *slthan(struct Value *left, struct Value *right)
{
    char *operand1 = NULL, *operand2 = NULL;
    switch (left->type)
    {
        case VREAL:
            sprintf(operand1, "%f", left->real);
            break;
        case VINT:
            sprintf(operand1, "%d", left->integer);
            break;
        case VSTRING:
            operand1 = left->string;
            break;
        default:
            InterpreterError("Booleans cannot be compared with string operators");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            sprintf(operand2, "%f", right->real);
            break;
        case VINT:
            sprintf(operand2, "%d", right->integer);
            break;
        case VSTRING:
            operand2 = right->string;
            break;
        default:
            InterpreterError("Booleans cannot be compared with string operators");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value *));
    result->bool = strcmp(operand1, operand2) < 0;
    result->type = VBOOL;
    return result;
}

struct Value *sgthan(struct Value *left, struct Value *right)
{
    char *operand1 = NULL, *operand2 = NULL;
    switch (left->type)
    {
        case VREAL:
            sprintf(operand1, "%f", left->real);
            break;
        case VINT:
            sprintf(operand1, "%d", left->integer);
            break;
        case VSTRING:
            operand1 = left->string;
            break;
        default:
            InterpreterError("Booleans cannot be compared with string operators");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            sprintf(operand2, "%f", right->real);
            break;
        case VINT:
            sprintf(operand2, "%d", right->integer);
            break;
        case VSTRING:
            operand2 = right->string;
            break;
        default:
            InterpreterError("Booleans cannot be compared with string operators");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value *));
    result->bool = strcmp(operand1, operand2) > 0;
    result->type = VBOOL;
    return result;
}

struct Value *nlthan(struct Value *left, struct Value *right)
{   
    double operand1 = 0, operand2 = 0;
    char *remainingCharacters = {'\0'};
    switch (left->type)
    {
        case VREAL:
            operand1 = left->real;
            break;
        case VINT:
            operand1 = left->integer;
            break;
        case VSTRING:
            if (!(operand1 = strtod(left->string, &remainingCharacters))) {
                InterpreterError("Invalid conversion from string to double");
                return NULL;
            }
            break;
        default:
            InterpreterError("Booleans cannot be compared with numerical operators");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            operand2 = right->real;
            break;
        case VINT:
            operand2 = right->integer;
            break;
        case VSTRING:
            if (!(operand2 = strtod(left->string, &remainingCharacters))) {
                InterpreterError("Invalid conversion from string to double");
                return NULL;
            }
            break;
        default:
            InterpreterError("Booleans cannot be compared with numerical operators");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value *));
    result->bool = operand1 < operand2;
    result->type = VBOOL;
    return result;
}

struct Value *ngthan(struct Value *left, struct Value *right)
{   
    double operand1 = 0, operand2 = 0;
    char *remainingCharacters = {'\0'};
    switch (left->type)
    {
        case VREAL:
            operand1 = left->real;
            break;
        case VINT:
            operand1 = left->integer;
            break;
        case VSTRING:
            if (!(operand1 = strtod(left->string, &remainingCharacters))) {
                InterpreterError("Invalid conversion from string to double");
                return NULL;
            }
            break;
        default:
            InterpreterError("Booleans cannot be compared with numerical operators");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            operand2 = right->real;
            break;
        case VINT:
            operand2 = right->integer;
            break;
        case VSTRING:
            if (!(operand2 = strtod(left->string, &remainingCharacters))) {
                InterpreterError("Invalid conversion from string to double");
                return NULL;
            }
            break;
        default:
            InterpreterError("Booleans cannot be compared with numerical operators");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value *));
    result->bool = operand1 > operand2;
    result->type = VBOOL;
    return result;
}

struct Value *plus(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value *));

    switch (left->type)
    {
        case VREAL:
            result->real = left->real;
            result->type = VREAL;
            break;
        case VINT:
            result->integer = left->integer;
            result->type = VINT;
            break;
        case VSTRING:
            InterpreterError("Strings not allowed for numerical operation (+)");
            return NULL;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (+)");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            if (result->type == VREAL)
                result->real += right->real;
            else
                result->real = result->integer + right->real;
            result->type = VREAL; 
            break;
        case VINT:
            if (result->type == VREAL) {
                result->real += right->integer;
            }
            else {
                result->integer += right->integer;
                result->type = VINT;
            } 
            break;
        case VSTRING:
            InterpreterError("Strings not allowed for numerical operation (+)");
            return NULL;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (+)");
            return NULL;
    }
    return result;
}

struct Value *minus(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value *));

    switch (left->type)
    {
        case VREAL:
            result->real = left->real;
            result->type = VREAL;
            break;
        case VINT:
            result->integer = left->integer;
            result->type = VINT;
            break;
        case VSTRING:
            InterpreterError("Strings not allowed for numerical operation (-)");
            return NULL;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (-)");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            if (result->type == VREAL)
                result->real -= right->real;
            else
                result->real = result->integer + right->real;
            result->type = VREAL; 
            break;
        case VINT:
            if (result->type == VREAL) {
                result->real -= right->integer;
            }
            else {
                result->integer -= right->integer;
                result->type = VINT;
            } 
            break;
        case VSTRING:
            InterpreterError("Strings not allowed for numerical operation (-)");
            return NULL;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (-)");
            return NULL;
    }
    return result;
}

struct Value *cat(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value *));
    
    char *operand1, operand2;

    switch (left->type)
    {
        case VREAL:
            sprintf(operand1, "%f", left->real);
            break;
        case VINT:
            sprintf(operand1, "%d", left->integer);
            break;
        case VSTRING:
            operand1 = left->string;
            break;
        default:
            InterpreterError("Booleans cannot be used for string operation (.)");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            sprintf(operand2, "%f", left->real);
            break;
        case VINT:
            sprintf(operand2, "%d", left->integer);
            break;
        case VSTRING:
            operand2 = left->string;
            break;
        default:
            InterpreterError("Booleans cannot be used for string operation (.)");
            return NULL;
    }
    strcpy(result->string, operand1);
    strcat(result->string, operand2);
    result->type = VSTRING;
    return result;
}

struct Value *mult(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value *));

    switch (left->type)
    {
        case VREAL:
            result->real = left->real;
            result->type = VREAL;
            break;
        case VINT:
            result->integer = left->integer;
            result->type = VINT;
            break;
        case VSTRING:
            InterpreterError("Strings cannot be used for numerical operation (*)");
            break;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (*)");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            if (result->type == VREAL)
                result->real *= right->real;
            else
                result->real = result->integer * right->real;
            result->type = VREAL; 
            break;
        case VINT:
            if (result->type == VREAL) {
                result->real *= right->integer;
            }
            else {
                result->integer *= right->integer;
                result->type = VINT;
            } 
            break;
        case VSTRING:
            InterpreterError("Strings not allowed for numerical operation (*)");
            return NULL;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (*)");
            return NULL;
    }
    return result;
}

struct Value *divide(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value *));

    switch (left->type)
    {
        case VREAL:
            result->real = left->real;
            result->type = VREAL;
            break;
        case VINT:
            result->integer = left->integer;
            result->type = VINT;
            break;
        case VSTRING:
            InterpreterError("Strings cannot be used for numerical operation (/)");
            break;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (/)");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            if (result->type == VREAL)
                result->real /= right->real;
            else
                result->real = result->integer / right->real;
            result->type = VREAL; 
            break;
        case VINT:
            if (result->type == VREAL) {
                result->real /= right->integer;
            }
            else {
                result->integer /= right->integer;
                result->type = VINT;
            } 
            break;
        case VSTRING:
            InterpreterError("Strings not allowed for numerical operation (/)");
            return NULL;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (/)");
            return NULL;
    }
    return result;
}

struct Value *divide(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value *));

    switch (left->type)
    {
        case VREAL:
            result->real = left->real;
            break;
        case VINT:
            result->integer = left->integer;
            break;
        case VSTRING:
            InterpreterError("Strings cannot be used for numerical operation (/)");
            break;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (/)");
            return NULL;
    }

    switch (right->type)
    {
        case VREAL:
            if (right->real == 0) {
                InterpreterError("Division by 0 is illegal");
                return NULL;
            }
            if (result->type == VREAL)
                result->real /= right->real;
            else 
                result->real = result->integer / right->real;
            break;
        case VINT:
            if (right->integer == 0) {
                InterpreterError("Division by 0 is illegal");
                return NULL;
            }
            if (result->type == VREAL)
                result->real /= right->integer;
            else
                result->integer /= right->integer;
            break;
        case VSTRING:
            InterpreterError("Strings cannot be for numerical operation (/)");
            return NULL;
        default:
            InterpreterError("Booleans cannot be used for numerical operation (/)");
            return NULL;
    }
    result->type = VREAL;
    return result;
}

struct Value *srepeat(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value *));
    
    char *operand1, operand2;

    if (left->type != VSTRING) { 
        InterpreterError("Numbers cannot be used for string operation (**)");
        return NULL;
    }
    if (right->type != VINT) { 
        InterpreterError("Only integers are allowed for right side of ** operator");
        return NULL;
    }
    
    strcpy(result->string, left->string);
    for (int i = 0; i < right->integer; i++)
        strcat(result->string, left->string);
    result->type = VSTRING;
    return result;
}