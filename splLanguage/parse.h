#ifndef PARSE_H_
#define PARSE_H_
#include <stdio.h>

//TOKEN is for handling extra  
enum StmtType{IFSTMT, ASSIGNSTMT, WRITELNSTMT, TOKEN, INVALID};

//All abstract syntax structs
struct ParseTree
{
    struct ParseStmtList *stmtList;
};

struct ParseStmtList
{
    struct ParseStmt **stmtArr;
};

struct ParseIfStmt
{
    struct ParseExpr *conditon;
    struct ParseStmtList *ifClause, *optionalElse;
};

struct ParseAssignStmt
{
    struct ParseVar *var;
    struct ParseExpr *expr;
};

struct ParseWriteLnStmt
{
    struct ParseExprList *exprList;
};

struct ParseStmt
{
    enum StmtType type;
    union
    {
        struct ParseIfStmt *ifStmt;
        struct ParseAssignStmt *assignStmt;
        struct ParseWriteLnStmt *writeLnStmt;
    };
};

struct ParseVar
{
    short int initialized;
    struct LexItem var;
};

struct ParseExprList
{
    struct ParseExpr **exprList;
};

struct ParseExpr
{
    struct ParseRelExpr *operand, *operand2;
    enum Token operator;
};

struct ParseRelExpr
{
    struct ParseAddExpr *operand, *operand2;
    enum Token operator;
};

struct ParseAddExpr
{
    struct ParseMultExpr **operandList;
    enum Token *operatorList;
};

struct ParseMultExpr
{
    struct ParseExponExpr **operandList;
    enum Token *operatorList;
};

struct ParseExponExpr
{
    struct ParseUnaryExpr **operandList;
    enum Token *operatorList;
};

struct ParseUnaryExpr
{
    struct ParsePrimaryExpr *operand;
    int sign; //-1 = negative, 1 = positive or not given
};

struct ParsePrimaryExpr
{
    union
    {
        struct LexItem ident;
        struct ParseExpr *expr;
        int integer;
        double real;
        char* string;
    };
};

//All functions for parsing
extern struct ParseTree *Prog(FILE* input, int* line);
extern struct ParseStmtList *StmtList(FILE* input, int* line);
extern struct ParseStmt *Stmt(FILE* input, int* line);
extern struct ParseWriteLnStmt *WriteLnStmt(FILE* input, int* line);
extern struct ParseIfStmt *IfStmt(FILE* input, int* line);
extern struct ParseAssignStmt *AssignStmt(FILE* input, int* line);
extern struct ParseVar *Var(FILE* input, int* line);
extern struct ParseExprList *ExprList(FILE* input, int* line);
extern struct ParseExpr *Expr(FILE* input, int* line);
extern struct ParseRelExpr *RelExpr(FILE* input, int* line);
extern struct ParseAddExpr *AddExpr(FILE* input, int* line);
extern struct ParseMultExpr *MultExpr(FILE* input, int* line);
extern struct ParseExponExpr *ExponExpr(FILE* input, int* line);
extern struct ParseUnaryExpr *UnaryExpr(FILE* input, int* line);
extern struct ParsePrimaryExpr *PrimaryExpr(FILE* input, int* line);
extern int ErrCount();
#endif