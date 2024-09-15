#include "interpreter.h"
#include "parse.h"
#include "operators.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct VariableList *head = NULL, *tail = NULL, *current = NULL;

unsigned short int Interpret(struct ParseTree *parseTree) {
    if (parseTree) {
        if (evaluateStmtList(parseTree->stmtList)) {
            printf("Successfully ran!\n"); 
            return 1;
        }
        else {
            printf("-------------------------------\nRuntime error(s) encountered\n");
            return 0;
        }
    }
    else {
        printf("-------------------------------\nInvalid Parse Tree, interpretation will not be done\n");
        return 0;
    }
}

unsigned short int evaluateStmtList(struct ParseStmtList *stmtList) {
    printf("======================================================\n");
    for (int i = 0; i < stmtList->length; i++) {
        if ((evaluateStmt(stmtList->stmtArr[i]))) {
            // printf("\nStatement %d successfully interpreted!\n", i+1);
            printf("-------------------------------------\n");
        }
        else 
        {
            // printf("\nStatement %d failed, runtime error detected :(\n", i+1);
            return 0;   
        }
    }
    printf("======================================================\n");
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
            printf("Runtime Error: Invalid if statement\n");
            return 0;
        }
    }
    else if (type == ASSIGNSTMT) {
        unsigned short int status = evaluateAssignStmt(stmt->assignStmt);
        if (status) {
            return 1;
        }
        else {
            printf("Runtime Error: Invalid assignment statement\n");
            return 0;
        }
    }
    else {
        unsigned short int status = evaluateWriteLnStmt(stmt->writeLnStmt);
        if (status) {
            return 1;
        }
        else {
            printf("Runtime Error: Invalid writeln statement\n");
            return 0;
        }
    }
    return 0;
}

unsigned short int evaluateIfStmt(struct ParseIfStmt *ifStmt) {
    struct Value *expr = evaluateExpr(ifStmt->condition);
    if (!expr) {
        printf("Runtime Error: Invalid conditional expression in if-statement\n");
        return 0;
    }
    
    if (expr->type != VBOOL) {
        printf("Runtime Error: Non-conditional expression in if-statement\n");
        return 0;
    }

    if (expr->bool == 1) {
        if(!evaluateStmtList(ifStmt->ifClause)) {
            printf("Runtime Error(s) detected in if-clause\n");
            return 0;
        }
        return 1;
    }
    else {
        if (!evaluateStmtList(ifStmt->elseClause)) {
            printf("Runtime Error(s) detected in else-clause\n");
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
            printf("Runtime Error: Invalid expression to assign to numeric variable (%s)\n", assign->var->variable);
        if (assign->var->variable[0] == '@')
            printf("Runtime Error: Invalid expression to assign to string variable (%s)\n", assign->var->variable);
        return 0;
    }

    struct VariableList *variableList = malloc(sizeof(struct VariableList));
    *variableList = (struct VariableList) {NULL, assign->var->variable, expression};

    if (expression->type == VSTRING && (assign->var->variable[0] == '$')) {
        printf("Runtime Error: Attempted to assign string (%s) to numeric variable (%s)\n"
        , expression->string, assign->var->variable);
        return 0;
    }

    else if ((assign->var->variable[0] == '@')) {
        if (expression->type == VREAL) {
            printf("Runtime Error: Attempted to assign real (%lf) to string variable (%s)\n"
            , expression->real, assign->var->variable);
            return 0;
        }
        else if (expression->type == VINT) {
            printf("Runtime Error: Attempted to assign integer (%d) to string variable (%s)\n"
            , expression->integer, assign->var->variable);   
            return 0;         
        }
    }

    else if (expression->type == VBOOL) {
        printf("Runtime Error: Attempted to assign boolean value to variable (%s)\n"
        , expression->string);
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
    
    printf("\n_______________________________________________\n\n");

    short unsigned int length = writeLn->exprList->length;
    for (int i = 0; i < length; i++) {
        switch (exprList[i]->type){
            case VINT: printf("%d", exprList[i]->integer); break;
            case VREAL: printf("%lf", exprList[i]->real); break;
            case VSTRING: printf("%s", exprList[i]->string); break;
            default: return 0;
        }
    }
    printf("\n_______________________________________________\n\n");
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
    return NULL;
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
    if (!addExpr) {
        return NULL;
    }
    struct Value *left = evaluateMultExpr(addExpr->left);
    if (!left) {
        return NULL;
    }
    
    if (!addExpr->right) {
        return left;
    }
    
    struct Value *right = evaluateAddExpr(addExpr->right);
    if (!right) {
        return NULL;
    }

    switch (addExpr->operator) {
        case PLUS: return plus(left, right);
        case MINUS: return minus(left, right);
        case CAT: return cat(left, right);
        default: return NULL;
    } 
}

struct Value *evaluateMultExpr(struct ParseMultExpr *multExpr) {
    if (!multExpr) {
        return NULL;
    }
    struct Value *left = evaluateExponExpr(multExpr->left);
    if (!left) {
        return NULL;
    }
    
    if (!multExpr->right) {
        return left;
    }
    
    struct Value *right = evaluateMultExpr(multExpr->right);
    if (!right) {
        return NULL;
    }

    switch (multExpr->operator) {
        case MULT: return mult(left, right);
        case DIV: return divide(left, right);
        case SREPEAT: return srepeat(left, right);
        default: return NULL;
    } 
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
                printf("Runtime Error: + or - before a string is invalid\n");
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
        case NVAR: case SVAR:
            struct VariableList *temp = head;
            while (temp) {
                if (strncmp(primaryExpr->ident, temp->variable, 128) == 0) {
                    expr = temp->value;
                    break;
                }
                temp = temp->next;
            }
            printf("Runtime Error: Attempted to use uninitialized variable (%s)\n", primaryExpr->ident);
        default:
            return NULL;
    }
    return expr;
}

