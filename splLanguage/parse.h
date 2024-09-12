#ifndef PARSE_H_
#define PARSE_H_
#include "lex.h"

// EXCEPTION is for handling extra
enum StmtType
{
    IFSTMT, ASSIGNSTMT, WRITELNSTMT, EXCEPTION, INVALID
};

enum ValType
{
    NVAR, SVAR, ID, EXPRESSION, INTEGER, REAL, STRING 
};

//All abstract syntax structs
struct ParseTree
{
    struct ParseStmtList *stmtList;
};

struct ParseStmtList
{
    struct ParseStmt **stmtArr;
    short unsigned int length;
};

struct ParseIfStmt
{
    struct ParseExpr *condition;
    struct ParseStmtList *ifClause, *elseClause;
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
    union
    {
        struct ParseIfStmt *ifStmt;
        struct ParseAssignStmt *assignStmt;
        struct ParseWriteLnStmt *writeLnStmt;
    };
    enum StmtType type; 
};

struct ParseVar
{
    char *variable;
};

struct ParseExprList
{
    struct ParseExpr **expr;
    short unsigned int length;
};

struct ParseExpr
{
    struct ParseRelExpr *left, *right;
    enum Token operator;
};

struct ParseRelExpr
{
    struct ParseAddExpr *left, *right;
    enum Token operator;
};

struct ParseAddExpr
{
    struct ParseMultExpr *left;
    struct ParseAddExpr *right;
    enum Token operator;
};

struct ParseMultExpr
{
    struct ParseExponExpr *left;
    struct ParseMultExpr *right;
    enum Token operator;
};

struct ParseExponExpr
{
    struct ParseUnaryExpr *left;
    struct ParseExponExpr *right;
    enum Token operator;
};

struct ParseUnaryExpr
{
    struct ParsePrimaryExpr *operand;
    short int sign; //-1 = negative, 1 = positive or not given
};

struct ParsePrimaryExpr
{
    union
    {
        char *ident; // NVAR, SVAR, ID
        struct ParseExpr *expr;
        int integer;
        double real;
        char* string;
    };
    enum ValType type;
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