#include "interpreter.h"
#include "parse.h"
#include "operators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct VariableList *head = NULL, *tail = NULL, *current = NULL;

unsigned short int Interpret(struct ParseTree *parseTree) {
    if (evaluateStmtList(parseTree->stmtList)) {
        printf("-------------------------------\n");
        printf("Program successfully run!\n");
        printf("-------------------------------\n"); 
        return 1;
    }
    else {
        printf("-------------------------------\n");
        printf("Interpreter error(s) encountered\n");
        printf("-------------------------------\n"); 
        return 0;
    }
}

unsigned short int evaluateStmtList(struct ParseStmtList *stmtList) {
    for (int i = 0; i < stmtList->length; i++) {
        if (!(evaluateStmt(stmtList->stmtArr[i])))
        {
            return 0;   
        }
    }
    return 1;
}

unsigned short int evaluateStmt(struct ParseStmt *stmt) {
    enum StmtType type = stmt->type;
    if (type == IFSTMT) {
        unsigned short int status = evaluateIfStmt(stmt->ifStmt);
        if (status) {
            return 1;
        }
        else {
            printf("Interpreter Error: Invalid if statement\n");
            return 0;
        }
    }
    else if (type == ASSIGNSTMT) {
        unsigned short int status = evaluateAssignStmt(stmt->assignStmt);
        if (status) {
            return 1;
        }
        else {
            printf("Interpreter Error: Invalid assignment statement\n");
            return 0;
        }
    }
    else {
        unsigned short int status = evaluateWriteLnStmt(stmt->writeLnStmt);
        if (status) {
            return 1;
        }
        else {
            printf("Interpreter Error: Invalid writeln statement\n");
            return 0;
        }
    }
    return 0;
}

unsigned short int evaluateIfStmt(struct ParseIfStmt *ifStmt) {
    struct Value *expr = evaluateExpr(ifStmt->condition);
    if (!expr) {
        printf("Interpreter Error: Invalid conditional expression in if-statement\n");
        return 0;
    }
    
    if (expr->type != VBOOL) {
        printf("Interpreter Error: Non-conditional expression in if-statement\n");
        return 0;
    }

    if (expr->bool == 1) {
        if(!evaluateStmtList(ifStmt->ifClause)) {
            printf("Interpreter Error(s) detected in if-clause\n");
            return 0;
        }
        return 1;
    }
    else {
        if (!evaluateStmtList(ifStmt->elseClause)) {
            printf("Interpreter Error(s) detected in else-clause\n");
            return 0;
        }
        return 1;
    }
    return 0;
}

unsigned short int evaluateAssignStmt(struct ParseAssignStmt *assign) {
    struct Value *expression = evaluateExpr(assign->expr);
    if (!expression) {
        if (assign->var->variable[0] == '$')
            printf("Interpreter Error: Invalid expression to assign to numeric variable (%s)\n", assign->var->variable);
        if (assign->var->variable[0] == '@')
            printf("Interpreter Error: Invalid expression to assign to string variable (%s)\n", assign->var->variable);
        return 0;
    }

    struct VariableList *variableList = malloc(sizeof(struct VariableList));
    
    struct VariableList *temp = head;
    
    // search to see if the variable already exists and update its value
    while (temp) {
        if (strncmp(assign->var->variable, temp->variable, 128) == 0) {
            temp->value = expression;
            break;
        }
        temp = temp->next;
    }

    *variableList = (struct VariableList) {NULL, assign->var->variable, expression};

    if (expression->type == VSTRING && (assign->var->variable[0] == '$')) {
        printf("Interpreter Error: Attempted to assign string (%s) to numeric variable (%s)\n"
        , expression->string, assign->var->variable);
        return 0;
    }

    if (expression->type == VBOOL) {
        printf("Interpreter Error: Attempted to assign boolean value to variable (%s)\n"
        , assign->var->variable);
        return 0;
    }
    
    if (!head) {
        head = variableList;
        current = head;
        return 1;
    }

    if (!head->next) {
        head->next = variableList;
        current = variableList;
        return 1;
    }

    current->next = variableList;
    current = current->next;

    return 1;
}

unsigned short int evaluateWriteLnStmt(struct ParseWriteLnStmt *writeLn) {
    struct Value **exprList = evaluateExprList(writeLn->exprList);
    if (!exprList) {
        return 0;
    }

    short unsigned int length = writeLn->exprList->length;
    for (int i = 0; i < length; i++) {
        switch (exprList[i]->type){
            case VINT: printf("%d", exprList[i]->integer); break;
            case VREAL: printf("%lf", exprList[i]->real); break;
            case VSTRING: printf("%s", exprList[i]->string); break;
            default: return 0;
        }
    }
    printf("\n");
    return 1;
}

struct Value **evaluateExprList(struct ParseExprList *exprList)
{
    struct Value **values = malloc(sizeof(struct Value) * exprList->length);
    for (int i = 0; i < exprList->length; i++) {
        struct Value *expr = evaluateExpr(exprList->expr[i]);
        if (!expr) {
            return NULL;
        }
        values[i] = expr;
    }
    return values;
}

struct Value *evaluateExpr(struct ParseExpr *expr) {
    struct Value *left = evaluateRelExpr(expr->left);
    if (!left) {
        return NULL;
    }
    
    if (!expr->right) {
        return left;
    }
    
    struct Value *right = evaluateRelExpr(expr->right);
    if (!right) {
        return NULL;
    }

    switch (expr->operator) {
        case SEQ: return seq(left, right);
        case NEQ: return neq(left, right);
        default: return NULL;
    }
}

struct Value *evaluateRelExpr(struct ParseRelExpr *relExpr) {
    struct Value *left = evaluateAddExpr(relExpr->left);
    if (!left) {
        return NULL;
    }
    
    if (!relExpr->right) {
        return left;
    }
    
    struct Value *right = evaluateAddExpr(relExpr->right);
    if (!right) {
        return NULL;
    }

    switch (relExpr->operator) {
        case SLTHAN: return slthan(left, right);
        case SGTHAN: return sgthan(left, right);
        case NLTHAN: return nlthan(left, right);
        case NGTHAN: return ngthan(left, right);
        default: return NULL;
    } 
}

struct Value *evaluateAddExpr(struct ParseAddExpr *addExpr) {
    struct ParseAddExpr *node = addExpr;
    struct Value *next = NULL, *value = NULL;
    enum Token operator = node->operator;

    while (node) {
        next = evaluateMultExpr(node->left);
        if (!next) {
            return NULL;
        }

        if (value == NULL) {
            value = next;
            node = node->right;
            continue;
        }

        switch (operator) {
            case PLUS: 
                value = plus(value, next);
                break;
            case MINUS: 
                value = minus(value, next);
                break;
            case CAT: 
                value = cat(value, next);
                break;
            default: 
                return NULL;
        }
        operator = node->operator;
        node = node->right;    
    }
    return value;
}

struct Value *evaluateMultExpr(struct ParseMultExpr *multExpr) {
    struct ParseMultExpr *node = multExpr;
    struct Value *next = NULL, *value = NULL;
    enum Token operator = node->operator;

    while (node) {
        next = evaluateExponExpr(node->left);
        if (!next) {
            return NULL;
        }

        if (value == NULL) {
            value = next;
            node = node->right;
            continue;
        }

        switch (operator) {
            case MULT: 
                value = mult(value, next);
                break;
            case DIV: 
                value = divide(value, next);
                break;
            case SREPEAT: 
                value = srepeat(value, next);
                break;
            default: 
                return NULL;
        }
        operator = node->operator;
        node = node->right;    
    }
    return value;
}

struct Value *evaluateExponExpr(struct ParseExponExpr *exponExpr) {
    if (!exponExpr) {
        return NULL;
    }
    struct Value *left = evaluateUnaryExpr(exponExpr->left);
    if (!left) {
        return NULL;
    }
    
    if (!exponExpr->right) {
        return left;
    }
    
    struct Value *right = evaluateExponExpr(exponExpr->right);
    if (!right) {
        return NULL;
    }

    if (exponExpr->operator == EXPONENT)
        return exponent(left, right);
    
    return NULL;
}

struct Value *evaluateUnaryExpr(struct ParseUnaryExpr *unaryExpr) {
    
    struct Value *operand = evaluatePrimaryExpr(unaryExpr->operand, unaryExpr->sign);
    if (!operand) {
        return NULL;
    }
    return operand;
}

struct Value *evaluatePrimaryExpr(struct ParsePrimaryExpr *primaryExpr, short int sign) {
    struct Value *expr = malloc(sizeof(struct Value));
    struct VariableList *temp = head;
    switch (primaryExpr->type) {
        case INTEGER:
            expr->type = VINT;
            expr->integer = primaryExpr->integer;
            if (sign != 0)
                expr->integer *= sign;
            break;
        case REAL:
            expr->type = VREAL;
            expr->real = primaryExpr->real;
            if (sign != 0)
                expr->real *= sign;
            break;
        case STRING:
            expr->type = VSTRING;
            if (sign != 0) {
                printf("Interpreter Error: + or - detected before string\n");
                return NULL;
            }
            expr->string = primaryExpr->string;
            break;
        case EXPRESSION: 
            expr = evaluateExpr(primaryExpr->expr);
            if (sign == 0)
                break;
            if (expr->type == VINT)
                expr->integer *= sign;
            else if (expr->type == VREAL)
                expr->real *= sign;
            break;
        case NVAR: 
            while (temp) {
                if (strncmp(primaryExpr->ident, temp->variable, 128) == 0) {
                    expr->type = temp->value->type;
                    if (expr->type == VREAL) {
                        expr->real = temp->value->real;
                        if (sign != 0) 
                            expr->real *= sign;
                    }
                    else {
                        expr->integer = temp->value->integer;
                        if (sign != 0) 
                            expr->integer *= sign;
                    }
                    return expr;
                }
                temp = temp->next;
            }
            printf("Interpreter Error: Attempted to use uninitialized variable (%s)\n", primaryExpr->ident);
            return NULL;        
        case SVAR:
            if (sign != 0) {
                printf("Interpreter Error: Signs are not allowed before string variables (%s)\n", primaryExpr->ident);
                return NULL;
            }
            while (temp) {
                if (strncmp(primaryExpr->ident, temp->variable, 128) == 0) {
                    expr = temp->value;
                    return expr;
                }
                temp = temp->next;
            }
            printf("Interpreter Error: Attempted to use uninitialized variable (%s)\n", primaryExpr->ident);
        default:
            return NULL;
    }
    return expr;
}