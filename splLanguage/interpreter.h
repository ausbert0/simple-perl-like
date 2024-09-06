#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "parse.h"
enum ValueTypes
{
    VINT, VREAL, VSTRING, VBOOL
};
struct Value 
{
    union 
    {
        double real;
        char *string;
        int integer;
        unsigned short int bool;
    };
    enum ValueTypes type;
};

unsigned short int interpret(struct ParseTree *);
unsigned short int evaluateStmtList(struct ParseStmtList *);
unsigned short int evaluateStmt(struct ParseStmt *);
unsigned short int evaluateIfStmt(struct ParseIfStmt *);
unsigned short int evaluateAssignStmt(struct ParseAssignStmt *);
unsigned short int evaluateWriteLnStmt(struct ParseWriteLnStmt *);

struct Value **evaluateExprList(struct ParseExprList *);

struct Value *evaluateExpr(struct ParseExpr *);

struct Value *evaluateRelExpr(struct ParseRelExpr *);

struct Value *evaluateAddExpr(struct ParseAddExpr *);

struct Value *evaluateMultExpr(struct ParseMultExpr *);

struct Value *evaluateExponExpr(struct ParseExponExpr *);

struct Value *evaluateUnaryExpr(struct ParseUnaryExpr *);

struct Value *evaluatePrimaryExpr(struct ParsePrimaryExpr *);

#endif