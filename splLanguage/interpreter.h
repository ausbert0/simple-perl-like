#ifndef INTERPRETER_H_
#define INTERPRETER_H_

unsigned short int interpret(struct ParseTree *);
unsigned short int evaluateStmtList(struct ParseStmtList *);
unsigned short int evaluateStmt(struct ParseStmt *);
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

#endif