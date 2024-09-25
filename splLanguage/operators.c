#include "interpreter.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
                printf("Interpreter Error: Invalid conversion from string to double\n");
                return NULL;
            }
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with numerical operators\n");
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
                printf("Interpreter Error: Invalid conversion from string to double\n");
                return NULL;
            }
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with numerical operators\n");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value));
    result->bool = operand1 == operand2;
    result->type = VBOOL;
    return result;
}

struct Value *seq(struct Value *left, struct Value *right)
{
    char operand1[128], operand2[128];
    switch (left->type)
    {
        case VREAL:
            snprintf(operand1, 128, "%lf", left->real);
            break;
        case VINT:
            snprintf(operand1, 128, "%d", left->integer);
            break;
        case VSTRING:
            strncpy(operand1, left->string, 128);
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with string operators\n");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            snprintf(operand2, 128, "%lf", right->real);
            break;
        case VINT:
            snprintf(operand2, 128, "%d", right->integer);
            break;
        case VSTRING:
            strncpy(operand2, right->string, 128);
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with string operators\n");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value));
    result->bool = strncmp(operand1, operand2, 128) == 0;
    result->type = VBOOL;
    return result;
}

struct Value *slthan(struct Value *left, struct Value *right)
{
    char operand1[128], operand2[128];
    switch (left->type)
    {
        case VREAL:
            snprintf(operand1, 128, "%lf", left->real);
            break;
        case VINT:
            snprintf(operand1, 128, "%d", left->integer);
            break;
        case VSTRING:
            strncpy(operand1, left->string, 128);
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with string operators\n");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            snprintf(operand2, 128, "%lf", right->real);
            break;
        case VINT:
            snprintf(operand2, 128, "%d", right->integer);
            break;
        case VSTRING:
            strncpy(operand2, right->string, 128);
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with string operators\n");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value));
    result->bool = strncmp(operand1, operand2, 128) < 0;
    result->type = VBOOL;
    return result;
}

struct Value *sgthan(struct Value *left, struct Value *right)
{
    char operand1[128], operand2[128];
    switch (left->type)
    {
        case VREAL:
            snprintf(operand1, 128, "%lf", left->real);
            break;
        case VINT:
            snprintf(operand1, 128, "%d", left->integer);
            break;
        case VSTRING:
            strncpy(operand1, left->string, 128);
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with string operators\n");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            snprintf(operand2, 128, "%lf", right->real);
            break;
        case VINT:
            snprintf(operand2, 128, "%d", right->integer);
            break;
        case VSTRING:
            strncpy(operand1, right->string, 128);
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with string operators\n");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value));
    result->bool = strncmp(operand1, operand2, 128) > 0;
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
                printf("Interpreter Error: Invalid conversion from string to double\n");
                return NULL;
            }
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with numerical operators\n");
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
                printf("Interpreter Error: Invalid conversion from string to double\n");
                return NULL;
            }
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with numerical operators\n");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value));
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
                printf("Interpreter Error: Invalid conversion from string to double\n");
                return NULL;
            }
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with numerical operators\n");
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
                printf("Interpreter Error: Invalid conversion from string to double\n");
                return NULL;
            }
            break;
        default:
            printf("Interpreter Error: Booleans cannot be compared with numerical operators\n");
            return NULL;
    }
    struct Value *result = malloc(sizeof(struct Value));
    result->bool = operand1 > operand2;
    result->type = VBOOL;
    return result;
}

struct Value *plus(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value));

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
            printf("Interpreter Error: Strings cannot be used for numerical operation (+)\n");
            return NULL;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (+)\n");
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
            printf("Interpreter Error: Strings cannot be used for numerical operation (+)\n");
            return NULL;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (+)\n");
            return NULL;
    }
    return result;
}

struct Value *minus(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value));

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
            printf("Interpreter Error: Strings cannot be used for numerical operation (-)\n");
            return NULL;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (-)\n");
            return NULL;
    }
    switch (right->type)
    {
        case VREAL:
            if (result->type == VREAL)
                result->real -= right->real;
            else
                result->real = result->integer - right->real;
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
            printf("Interpreter Error: Strings cannot be used for numerical operation (-)\n");
            return NULL;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (-)\n");
            return NULL;
    }
    return result;
}

struct Value *cat(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value));
    
    char operand1[512], operand2[512];

    switch (left->type)
    {
        case VREAL:
            snprintf(operand1, 512, "%lf", left->real);
            break;
        case VINT:
            snprintf(operand1, 512, "%d", left->integer);
            break;
        case VSTRING:
            strncpy(operand1, left->string, 512);
            
            break;
        default:
            printf("Interpreter Error: Booleans cannot be used for string operation (.)\n");
            return NULL;
    }
    operand1[511] = '\0';

    switch (right->type)
    {
        case VREAL:
            snprintf(operand2, 512, "%lf", right->real);
            break;
        case VINT:
            snprintf(operand2, 512, "%d", right->integer);
            break;
        case VSTRING:
            strncpy(operand2, right->string, 512);
            break;
        default:
            printf("Interpreter Error: Booleans cannot be used for string operation (.)\n");
            return NULL;
    }
    operand2[511] = '\0';

    result->string = malloc(sizeof(char) * 1024);
    result->string[1023] = '\0';
    strncpy(result->string, operand1, 512);
    strncat(result->string, operand2, 512);
    
    result->type = VSTRING;
    return result;
}

struct Value *mult(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value));

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
            printf("Interpreter Error: Strings cannot be used for numerical operation (*)\n");
            break;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (*)\n");
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
            printf("Interpreter Error: Strings cannot be used for numerical operation (*)\n");
            return NULL;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (*)\n");
            return NULL;
    }
    return result;
}

struct Value *divide(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value));

    switch (left->type)
    {
        case VREAL:
            result->real = left->real;
            break;
        case VINT:
            result->real = left->integer;
            break;
        case VSTRING:
            printf("Interpreter Error: Strings cannot be used for numerical operation (/)\n");
            break;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (/)\n");
            return NULL;
    }

    switch (right->type)
    {
        case VREAL:
            if (right->real == 0) {
                printf("Runtime Error: Division by 0 is illegal\n");
                return NULL;
            }
            result->real /= right->real;
            break;
        case VINT:
            if (right->integer == 0) {
                printf("Runtime Error: Division by 0 is illegal\n");
                return NULL;
            }
            result->real /= right->integer;
            break;
        case VSTRING:
            printf("Interpreter Error: Strings cannot be for numerical operation (/)\n");
            return NULL;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (/)\n");
            return NULL;
    }
    result->type = VREAL;
    return result;
}

struct Value *srepeat(struct Value *left, struct Value *right)
{   
    if (left->type != VSTRING) { 
        printf("Interpreter Error: Numbers cannot be used for string operation (**)\n");
        return NULL;
    }

    int repeat = 0;

    switch (right->type) 
    {
        case VINT:
            repeat = right->integer-1;
            break;
        case VREAL:
            printf("WARNING: Real value used for string repeat will be truncated\n");
            repeat = right->real-1;
            break;
        default:
            printf("Interpreter Error: Non-numeric value detected on right side of ** operator\n");
            return NULL;
    }

    struct Value *result = malloc(sizeof(struct Value));
    result->string = malloc(sizeof(char) * 1024);
    result->string[1023] = '\0';
    strncpy(result->string, left->string, 512);
    for (int i = 0; i < repeat; i++)
        strncat(result->string, left->string, 512);
    result->type = VSTRING;
    return result;
}

struct Value *exponent(struct Value *left, struct Value *right)
{   
    struct Value *result = malloc(sizeof(struct Value));

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
            printf("Interpreter Error: Strings cannot be used for numerical operation (^)\n");
            break;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (^)\n");
            return NULL;
    }

    switch (right->type)
    {
        case VREAL:
            if (result->type == VREAL) {
                if (right->real == 0 && result->real == 0) {
                    printf("Runtime Error: 0^0 is not a valid expression\n");
                    return NULL;
                }
                result->real = pow(result->real, right->real);
            }
            else {
                if (right->real == 0 && result->integer == 0) {
                    printf("Runtime Error: 0^0 is not a valid expression\n");
                    return NULL;
                }
                result->real = pow(result->integer, right->real);
            }
            result->type = VREAL;
            break;
        case VINT:
            if (result->type == VREAL) {
                if (right->integer == 0 && result->real == 0) {
                    printf("Runtime Error: 0^0 is not a valid expression\n");
                    return NULL;
                }
                result->type = VREAL;
                result->real = pow(result->real, right->integer);
            }
            else {
                if (right->integer == 0 && result->integer == 0) {
                    printf("Runtime Error: 0^0 is not a valid expression\n");
                    return NULL;
                }
                result->type = VINT;
                result->integer = pow(result->integer, right->integer);
            }
            break;
        case VSTRING:
            printf("Interpreter Error: Strings cannot be used for numerical operation (^)\n");
            return NULL;
        default:
            printf("Interpreter Error: Booleans cannot be used for numerical operation (^)\n");
            return NULL;
    }
    return result;
}