#include "interpreter.h"
#include "parse.h"
#include <stdio.h>

unsigned short interpret(struct ParseTree *parseTree) {
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

unsigned short evaluateStmtList(struct ParseStmtList *stmtList) {
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


unsigned short int evaluateIfStmt(struct ParseIfStmt *);


unsigned short int evaluateAssignStmt(struct ParseAssignStmt *);


unsigned short int evaluateWriteLnStmt(struct ParseWriteLnStmt *);



unsigned short int evaluateExprList(struct ParseExprList *);


unsigned short int evaluateExpr(struct ParseExpr *);



unsigned short int evaluateRelExpr(struct ParseRelExpr *);



unsigned short int evaluateAddExpr(struct ParseAddExpr *);



unsigned short int evaluateMultExpr(struct ParseMultExpr *);



unsigned short int evaluateExponExpr(struct ParseExponExpr *);



unsigned short int evaluateUnaryExpr(struct ParseUnaryExpr *);



unsigned short int evaluatePrimaryExpr(struct ParsePrimaryExpr *);

