#include <stdlib.h>
#include "lex.h"
#include "parse.h"
#include <string.h>

unsigned short int error_count = 0, pushed_back = 0;
struct LexItem *pushed_token = NULL;

size_t varIndex = 0;

void ParseError(int* line, char* msg)
{
    ++error_count;
    printf("Parser Error %d) Line # %d: %s\n", error_count, *line, msg);
}

int ErrCount()
{
    return error_count;
}

struct LexItem* GetNextToken(FILE* input, int* line)
{
    if (pushed_back)
    {
        pushed_back = 0;
        return pushed_token;
    }
    return getNextToken(input, line);
}

void PushBackToken(struct LexItem *lex)
{
    if (pushed_back)
        abort();
    pushed_back = 1;
    pushed_token = lex;
}

//Program is: Prog ::= StmtList
struct ParseTree *Prog(FILE* input, int* line)
{
    struct ParseTree *parser = malloc(sizeof(struct ParseTree *));
    struct ParseStmtList *stmtList = StmtList(input, line);
    if (stmtList && (GetNextToken(input, line))->token == DONE)
    {
        *parser = (struct ParseTree) {stmtList};
        printf("(DONE)\n");
        return parser;
    }
    ParseError(line, "Missing Program");
    return NULL;
}

//StmtList ::= Stmt;{Stmt;}
struct ParseStmtList *StmtList(FILE* input, int* line)
{
    struct ParseStmt *stmt = NULL;
    struct ParseStmt **stmtArr = malloc(sizeof(struct ParseStmt *) * 64); // 64 stmts
    short unsigned int index = 0;
    struct ParseStmtList *stmtList = malloc(sizeof(struct ParseStmtList *)); 
    struct LexItem *lex = NULL;
    while ((stmt = Stmt(input, line)))
    {
        if (index == 64) // stop when max number of statements is hit
        {
            ParseError(line, "Statement limit reached");
            return stmtList;
        }
        stmtArr[index] = NULL;
        if (stmt->type != EXCEPTION)
            stmtArr[index++] = stmt;
        lex = GetNextToken(input, line);
        if (lex->token == RBRACES || lex->token == DONE)
        {
            PushBackToken(lex);
            *stmtList = (struct ParseStmtList) {stmtArr, index};
            return stmtList;
        }
        if (lex->token == ELSE)
        {
            ParseError(line, "Missing right brace");
            return NULL;
        }
        if (lex->token != SEMICOL)
        {
            ParseError(line, "Missing semicolon at end of statement");
            return NULL;         
        }
    }
    stmtArr[index] = NULL;
    ParseError(line, "Syntactic error in program body");
    return NULL;
}

//Stmt ::= AssignStmt | WriteLnStmt | IfStmt
struct ParseStmt *Stmt(FILE* input, int* line)
{
    struct ParseStmt *stmt = malloc(sizeof(struct ParseStmt *)); 
    struct LexItem* lex = GetNextToken(input, line);
    switch (lex->token)
    {
        case IF:
        {
            struct ParseIfStmt *ifStmt = IfStmt(input, line);
            if (!ifStmt)
            {
                ParseError(line, "Incorrect if-statement");
                return NULL;
            }
            stmt->type = IFSTMT;
            stmt->ifStmt = ifStmt;
            return stmt;
        }
        case NIDENT: case SIDENT:
        {
            PushBackToken(lex);
            struct ParseAssignStmt *assignStmt = AssignStmt(input, line);
            if (!assignStmt)
            {
                ParseError(line, "Incorrect assignment statement");
                return NULL;
            }
            stmt->type = ASSIGNSTMT;
            stmt->assignStmt = assignStmt;
            return stmt;
        }
        case WRITELN:
        {
            struct ParseWriteLnStmt *writeLnStmt = WriteLnStmt(input, line);
            if (!writeLnStmt)
            {
                ParseError(line, "Incorrect writeln Statement");
                return NULL;
            }
            stmt->writeLnStmt = writeLnStmt;
            stmt->type = WRITELNSTMT;
            return stmt;
        }
        case IDENT:
            ParseError(line, "Unrecognized identifier");
            return NULL;
        // case ELSE:
        //     PushBackToken(lex); 
        //     stmt->type = EXCEPTION;
        //     return stmt;   
        default:
            PushBackToken(lex); 
            stmt->type = EXCEPTION;
            return stmt; 
    }
    return stmt;
}

//WritelnStmt ::= writeln (ExprList)
struct ParseWriteLnStmt *WriteLnStmt(FILE* input, int* line)
{
    struct ParseWriteLnStmt *writeStmt = malloc(sizeof(struct ParseWriteLnStmt *));
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token != LPAREN)
    {
       ParseError(line, "Missing left parenthesis of writeln statement");
       return NULL; 
    }
    struct ParseExprList *exprList = ExprList(input, line);
    if (!exprList)
    {
        ParseError(line, "Invalid expression list");
        return NULL;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RPAREN)
    {
        ParseError(line, "Missing right parenthesis of writeln statement");
        return NULL;
    }
    *writeStmt = (struct ParseWriteLnStmt) {exprList};
    return writeStmt;
}

//ExprList:= Expr {,Expr}
struct ParseExprList *ExprList(FILE* input, int* line)
{
    struct ParseExpr **exprArr = malloc(sizeof(struct exprArr *) * 16);
    size_t length = 0;
    struct ParseExprList *exprList = malloc(sizeof(struct ParseExprList *));
    struct LexItem* lex;

    while ((exprArr[length++] = Expr(input, line)))
    {
        lex = GetNextToken(input, line);
        if (lex->token != COMMA)
        {
            if (lex->token != RPAREN)
            {
                ParseError(line, "Missing comma in writeln expression list");
                return NULL;
            }
            exprArr[length] = NULL;
            PushBackToken(lex);
            *exprList = (struct ParseExprList) {exprArr, length};
            return exprList;
        }
    }
    exprArr[length] = NULL;
    ParseError(line, "Missing Expression");
    return NULL;
}

//IfStmt ::= if (Expr) '{' StmtList '}' [ else '{' StmtList '}' ] 
struct ParseIfStmt *IfStmt(FILE* input, int* line)
{
    struct ParseIfStmt *IfStmt = malloc(sizeof(struct ParseIfStmt *));
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token != LPAREN)
    {
        ParseError(line, "Missing left parenthesis of If-Statement");
        return NULL;
    }
    struct ParseExpr *condition = Expr(input, line);
    if (!condition)
    {
        ParseError(line, "Invalid expression for evaluation in If-Statement");
        return NULL;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RPAREN)
    {
        ParseError(line, "Missing right parenthesis of If-Statement");
        return NULL;
    }
    lex = GetNextToken(input, line);
    if (lex->token != LBRACES)
    {
        ParseError(line, "Missing left brace of If-Clause");
        return NULL;
    }
    struct ParseStmtList *trueBody = StmtList(input, line);
    if (!trueBody)
    {
        ParseError(line, "Invalid statement list in If-Clause");
        return NULL;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RBRACES)
    {
        ParseError(line, "Missing right brace of If-Clause");
        return NULL;
    }
    lex = GetNextToken(input, line);
    if (lex->token != ELSE)
    {
        PushBackToken(lex);
        *IfStmt = (struct ParseIfStmt) {condition, trueBody, NULL};
        return IfStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != LBRACES)
    {
        ParseError(line, "Missing left brace of Else-Clause");
        return NULL;
    }
    struct ParseStmtList *falseBody = StmtList(input, line);
    if (!falseBody)
    {
        ParseError(line, "Invalid statement list for Else-Clause");
        return NULL;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RBRACES)
    {
        ParseError(line, "Missing right brace of Else-Clause");
        return NULL;
    }
    *IfStmt = (struct ParseIfStmt) {condition, trueBody, falseBody};
    return IfStmt;
}

//AssignStmt ::= Var = Expr
struct ParseAssignStmt *AssignStmt(FILE* input, int* line)
{
    struct ParseAssignStmt *assignStmt = malloc(sizeof(struct AssignStmt *));
    struct ParseVar *var = Var(input, line);
    if (!var)
    {
        return NULL;
    }
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token != ASSOP)
    {
        ParseError(line, "Missing assignment operator");
        return NULL;
    }
    struct ParseExpr *expr = Expr(input, line);
    if (!expr)
    {
        ParseError(line, "Missing expression in assignment statement");
        return NULL;
    }
    ;
    *assignStmt = (struct ParseAssignStmt) {var, expr};
    return assignStmt;
}

//Var ::= NIDENT | SIDENT
struct ParseVar *Var(FILE* input, int* line)
{
    struct LexItem *lex = GetNextToken(input, line);
    struct ParseVar *var = malloc(sizeof(struct ParseVar *));
    if (lex->token == NIDENT || lex->token == SIDENT)
    {
        *var = (struct ParseVar) {lex};
        return var;
    }
    ParseError(line, "Invalid variable name");
    return NULL;
}

//Expr ::= RelExpr [(-eq|==) RelExpr]
struct ParseExpr *Expr(FILE* input, int* line)
{
    struct ParseRelExpr *relExpr1 = RelExpr(input, line), *relExpr2;

    if (!relExpr1)
    {
        return NULL;
    }
    
    struct ParseExpr *expr = malloc(sizeof(struct ParseExpr *));
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token == NEQ || lex->token == SEQ)
    {
        relExpr2 = RelExpr(input, line);
        if (!relExpr2)
        {
            ParseError(line, "Missing operand after operator");
            return NULL;
        }
        *expr = (struct ParseExpr) {relExpr1, relExpr2, lex->token};
        return expr;
    }
    PushBackToken(lex);
    *expr = (struct ParseExpr) {relExpr1, NULL, ERR};
    return expr;
}

//RelExpr ::= AddExpr [ ( -lt | -gt | < | > )  AddExpr ]
struct ParseRelExpr *RelExpr(FILE* input, int* line)
{
    struct ParseAddExpr *addExpr1 = AddExpr(input, line), *addExpr2;
    if (!addExpr1)
    {
        return NULL;
    }
    
    struct ParseRelExpr *relExpr = malloc(sizeof(struct ParseRelExpr *));
    struct LexItem* lex = GetNextToken(input, line);
    
    if (lex->token == NLTHAN || lex->token == NGTHAN 
    || lex->token == SLTHAN || lex->token == SGTHAN)
    {
        addExpr2 = AddExpr(input, line);
        if (!addExpr2)
        {
            ParseError(line, "Missing operand after operator");
            return NULL;
        }
        *relExpr = (struct ParseRelExpr) {addExpr1, addExpr2, lex->token};
        return relExpr;
    }
    PushBackToken(lex);
    *relExpr = (struct ParseRelExpr) {addExpr1, NULL, ERR};
    return relExpr;
}

//AddExpr ::= MultExpr { ( + | - | .) MultExpr }
struct ParseAddExpr *AddExpr(FILE* input, int* line)
{
    struct ParseMultExpr *multExpr = MultExpr(input, line); // current MultExpr

    if (!multExpr)
    {
        return NULL;
    }
    
    struct ParseAddExpr *addExprAST = malloc(sizeof(struct ParseAddExpr *));
    addExprAST->left = multExpr;
    addExprAST->right = NULL; // accounts for no operator if none are found
    
    // to keep track of the current AddExpr
    struct ParseAddExpr *addExpr = addExprAST;

    struct LexItem* lex = GetNextToken(input, line);
    while (lex->token == PLUS || lex->token == MINUS || lex->token == CAT)
    {
        addExpr->right = malloc(sizeof(struct ParseAddExpr *));
        multExpr = MultExpr(input, line);
        if (!multExpr)
        {
            ParseError(line, "Missing operand after operator");
            return NULL;
        }
        addExpr->operator = lex->token;
        addExpr->left = multExpr;
        addExpr = addExpr->right;
        addExpr->right = NULL;
        lex = GetNextToken(input, line);
    }
    PushBackToken(lex);
    return addExprAST;
}

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
struct ParseMultExpr *MultExpr(FILE* input, int* line)
{
    struct ParseExponExpr *exponExpr = ExponExpr(input, line); // current exponExpr 
    if (!exponExpr)
    {  
        return NULL;
    }
    
    struct ParseMultExpr *multExprAST = malloc(sizeof(struct ParseMultExpr *));
    multExprAST->right = NULL;
    multExprAST->left = exponExpr;
    
    // to keep track of the current MultExpr
    struct ParseMultExpr *multExpr = multExprAST;

    struct LexItem* lex = GetNextToken(input, line);
    while (lex->token == MULT || lex->token == DIV || lex->token == SREPEAT)
    {
        multExpr->right = malloc(sizeof(struct ParseMultExpr *));
        exponExpr = ExponExpr(input, line);
        if (!exponExpr)
        {
            ParseError(line, "Missing operand after operator");
            return NULL;
        }
        multExpr->operator = lex->token;
        multExpr->left = exponExpr;
        multExpr = multExpr->right;
        multExpr->right = NULL;
        lex = GetNextToken(input, line);
    }
    PushBackToken(lex);
    return multExprAST;
}

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
struct ParseExponExpr *ExponExpr(FILE* input, int* line)
{
    struct ParseUnaryExpr *unaryExpr = UnaryExpr(input, line);
    if (!unaryExpr)
    {  
        return NULL;
    }
    
    struct ParseExponExpr *exponExprAST = malloc(sizeof(struct ParseExponExpr *));
    exponExprAST->right = NULL;
    exponExprAST->left = unaryExpr;
    
    // to keep track of the current ExponExpr
    struct ParseExponExpr *exponExpr = exponExprAST;
    struct LexItem* lex = GetNextToken(input, line);
    while (lex->token == EXPONENT)
    {
        exponExpr->right = malloc(sizeof(struct ParseExponExpr *));
        unaryExpr = UnaryExpr(input, line);
        if (!unaryExpr)
        {
            ParseError(line, "Missing operand after operator");
            return NULL;
        }
        exponExpr->operator = lex->token;
        exponExpr->left = unaryExpr;
        exponExpr = exponExpr->right;
        exponExpr->right = NULL;
        lex = GetNextToken(input, line);
    }
    PushBackToken(lex);
    return exponExpr;
}

//UnaryExpr ::= [( - | + )] PrimaryExpr
struct ParseUnaryExpr *UnaryExpr(FILE* input, int* line)
{
    struct ParseUnaryExpr *unaryExpr = malloc(sizeof(struct ParseUnaryExpr *));
    int sign = 0;
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token == PLUS)
        sign = 1;
    else if (lex->token == MINUS)
        sign = -1;
    else 
        PushBackToken(lex);
    struct ParsePrimaryExpr *primaryExpr = PrimaryExpr(input, line);
    if (!primaryExpr)
    {
        return NULL;
    }
    *unaryExpr = (struct ParseUnaryExpr) {primaryExpr, sign};
    return unaryExpr;
}

//PrimaryExpr ::= IDENT | NIDENT | SIDENT | ICONST | RCONST | SCONST | (Expr)
struct ParsePrimaryExpr *PrimaryExpr(FILE* input, int* line)
{
    struct ParsePrimaryExpr *primaryExpr = malloc(sizeof(struct ParsePrimaryExpr *));
    struct ParseExpr *expr; 
    struct LexItem *lex = GetNextToken(input, line);
    switch (lex->token)
    {
        case ICONST:
            primaryExpr->integer = atoi(lex->lexeme);
            primaryExpr->type = INTEGER;
            return primaryExpr;
        case RCONST:
            primaryExpr->real = atof(lex->lexeme);
            primaryExpr->type = REAL;
            return primaryExpr;
        case SCONST:
            primaryExpr->string = lex->lexeme;
            primaryExpr->type = STRING;
            return primaryExpr;
        case IDENT: 
            primaryExpr->ident = lex;
            primaryExpr->type = ID;
            return primaryExpr;        
        case NIDENT: 
            primaryExpr->ident = lex;
            primaryExpr->type = NVAR;
            return primaryExpr;
        case SIDENT:
            primaryExpr->ident = lex;
            primaryExpr->type = SVAR;
            return primaryExpr;
        case LPAREN:
            expr = Expr(input, line);
            if(!expr)
            {
                ParseError(line, "Missing expression after left parenthesis of expression");
                return NULL;
            }
            lex = GetNextToken(input, line);
            if (lex->token != RPAREN)
            {
                ParseError(line, "Missing closing parenthesis of expression");
                return NULL;
            }
            primaryExpr->expr = expr;
            primaryExpr->type = EXPRESSION;
            return primaryExpr;
        default:
            return NULL;
    }
}