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
            InterpreterError("Booleans cannot be compared with numerical operators (left side of operator)");
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
            InterpreterError("Booleans cannot be compared with numerical operators (right side of operator)");
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
            InterpreterError("Booleans cannot be compared with string operators (left side of operator)");
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
            InterpreterError("Booleans cannot be compared with string operators (right side of operator)");
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
            InterpreterError("Booleans cannot be compared with string operators (left side of operator)");
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
            InterpreterError("Booleans cannot be compared with string operators (right side of operator)");
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
            InterpreterError("Booleans cannot be compared with string operators (left side of operator)");
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
            InterpreterError("Booleans cannot be compared with string operators (right side of operator)");
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
            InterpreterError("Booleans cannot be compared with numerical operators (left side of operator)");
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
            InterpreterError("Booleans cannot be compared with numerical operators (right side of operator)");
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
            InterpreterError("Booleans cannot be compared with numerical operators (left side of operator)");
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
            InterpreterError("Booleans cannot be compared with numerical operators (right side of operator)");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value *));
    result->bool = operand1 > operand2;
    result->type = VBOOL;
    return result;
}

// struct Value *add(struct Value *left, struct Value *right)
// {   
//     double operand1 = 0, operand2 = 0;
//     switch (left->type)
//     {
//         case VREAL:
//             operand1 = left->real;
//             break;
//         case VINT:
//             operand1 = left->integer;
//             break;
//         case VSTRING:
//             InterpreterError("Strings not allowed for numerical operation (+)");
//             return NULL;
//         default:
//             InterpreterError("Booleans cannot be used for numerical operation (+)");
//             return NULL;
//     }
//     switch (right->type)
//     {
//         case VREAL:
//             operand2 = right->real;
//             break;
//         case VINT:
//             operand2 = right->integer;
//             break;
//         case VSTRING:
//             InterpreterError("Strings not allowed for numerical operation (+)");
//             return NULL;
//         default:
//             InterpreterError("Booleans cannot be used for numerical operation (+)");
//             return NULL;
//     }
//     struct Value *result = malloc(sizeof(struct Value *));
//     result->bool = operand1 > operand2;
//     result->type = VBOOL;
//     return result;
// }