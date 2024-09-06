#include "interpreter.h"
#include "parse.h"
#include "operators.h"
#include <stdio.h>
#include <stdlib.h>

unsigned short int interpret(struct ParseTree *parseTree) {
    if (parseTree) {
        if (evaluateStmtList(parseTree->stmtList)) {
            printf("Successfully ran!\n"); 
            return 1;
        }
        else {
            printf("Runtime error(s) encountered\n");
            return 0;
        }
    }
    else {
        printf("Invalid Parse Tree, interpretation will not be done\n");
        return 0;
    }
}

unsigned short int evaluateStmtList(struct ParseStmtList *stmtList) {
    printf("======================================================\n");
    for (int i = 0; i < stmtList->length; i++) {
        if ((evaluateStmt(stmtList->stmtArr[i]))) {
            printf("Statement %d successfully interpreted!\n", i+1);
            printf("-------------------------------------");
        }
        else 
        {
            printf("Statement %d failed, runtime error detected :(\n", i+1);
            return 0;   
        }
    }
    printf("======================================================\n");
    return 1;
}

unsigned short int evaluateStmt(struct ParseStmt *stmt) {
    enum StmtType type = stmt->type;
    if (type == IFSTMT) {
        return evaluateIfStmt(stmt->ifStmt);
    }
    else if (type == ASSIGNSTMT) {
        return evaluateAssignStmt(stmt->assignStmt);
    }
    else {
        return evaluateWriteLnStmt(stmt->writeLnStmt);
    }
    return 0;
}


unsigned short int evaluateIfStmt(struct ParseIfStmt *ifStmt) {
    struct Value *expr = evaluateExpr(ifStmt->condition);
    if (!expr) {return 0;}
    
    if (expr->type != VBOOL) {
        InterpreterError("Non-conditional expression in if-statement");
        return 0;
    }

    if (expr->bool == 1) {
        return evaluateStmtList(ifStmt->ifClause);
    }
    return evaluateStmtList(ifStmt->elseClause);
}

unsigned short int evaluateAssignStmt(struct ParseAssignStmt *);


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
    return 1;
}


struct Value **evaluateExprList(struct ParseExprList *exprList)
{
    struct Value **values = malloc(sizeof(struct Value **) * exprList->length);
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

    switch (exponExpr->operator) {
        case EXPONENT: return exponent(left, right);
        default: return NULL;
    } 
}


struct Value *evaluateUnaryExpr(struct ParseUnaryExpr *unaryExpr) {
    struct Value *operand = evaluatePrimaryExpr(unaryExpr->operand);
    if (!operand) {
        return NULL;
    }

    if ((unaryExpr->sign = -1 || unaryExpr->sign == 1) && operand->type == VSTRING) {
        InterpreterError("Strings cannot have signs");
        return NULL;
    }

    return signedExpr(operand, unaryExpr->sign);
}


struct Value *evaluatePrimaryExpr(struct ParsePrimaryExpr *primaryExpr) {
    struct Value *expr = malloc(sizeof(struct Value *));
    switch (primaryExpr->type) {
        case INTEGER:
            expr->type = VINT;
            expr->integer = primaryExpr->integer;
            break;
        case REAL:
            expr->type = VREAL;
            expr->real = primaryExpr->real;
            break;
        case STRING:
            expr->type = VSTRING;
            expr->string = primaryExpr->string;
            break;
        case EXPRESSION: 
            expr = evaluateExpr(primaryExpr->expr);
        case NVAR: case SVAR: //tf do i do here
        default:
            return NULL;
    }
    return expr;
}

