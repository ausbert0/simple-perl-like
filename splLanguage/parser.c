#include <stdlib.h>
#include "lex.h"
#include "parse.h"
#include <string.h>

unsigned short int error_count = 0, pushed_back = 0;
size_t varIndex = 0;
struct LexItem *pushed_token = NULL;
struct ParseVar variables[10]; //figure out whether to malloc/calloc

void ParseError(int* line, char* msg)
{
    ++error_count;
    printf("Parser Error %d. Line # %d: %s\n", error_count, *line, msg);
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
    struct ParseTree *parser;
    struct ParseStmtList *stmtList = StmtList(input, line);
    if (stmtList && (GetNextToken(input, line))->token == DONE)
    {
        *parser = (struct ParseTree) {stmtList};
        printf("(DONE)\n");
        return parser;
    }
    ParseError(line, "Missing Program");
    return 0;
}

//StmtList ::= Stmt;{Stmt;}
struct ParseStmtList *StmtList(FILE* input, int* line)
{
    struct ParseStmt *stmt, *stmtArr[30];
    size_t index = 0;
    struct ParseStmtList *stmtList; 
    struct LexItem *lex;
    while ((stmt = Stmt(input, line))->type != INVALID)
    {
        if (stmt != NULL)
            stmtArr[index++] = stmt;
        lex = GetNextToken(input, line);
        if (lex->token == RBRACES || lex->token == DONE)
        {
            stmtArr[index] = NULL;
            PushBackToken(lex);
            *stmtList = (struct ParseStmtList) {0};
            return stmtList;
        }
        if (lex->token == ELSE)
        {
            ParseError(line, "Missing right brace");
            *stmtList = (struct ParseStmtList) {0, stmtArr};
            return stmtList;
        }
        if (lex->token != SEMICOL)
        {
            ParseError(line, "Missing semicolon at end of statement");
            *stmtList = (struct ParseStmtList) {0, stmtArr};
            return stmtList;         
        }
    }
    stmtArr[index] = NULL;
    ParseError(line, "Syntactic error in program body");
    *stmtList = (struct ParseStmtList) {0, stmtArr};
    return stmtList;
}

//Stmt ::= AssignStmt | WriteLnStmt | IfStmt
struct ParseStmt *Stmt(FILE* input, int* line)
{
    struct ParseStmt temp, *stmt = &temp; 
    struct LexItem* lex = GetNextToken(input, line);
    switch (lex->token)
    {
        case IF:
        {
            struct ParseIfStmt *ifStmt = IfStmt(input, line);
            stmt->ifStmt = ifStmt;
            if (!ifStmt->status)
            {
                ParseError(line, "Incorrect If-Statement");
                stmt->type = INVALID;
                return stmt;
            }
            stmt->type = IFSTMT;
            return stmt;
        }
        case NIDENT: case SIDENT:
        {
            PushBackToken(lex);
            struct ParseAssignStmt *assignStmt = AssignStmt(input, line);
            stmt->assignStmt = assignStmt;
            if (!assignStmt->status)
            {
                ParseError(line, "Incorrect Assignment Statement");
                stmt->type = INVALID;
                return stmt;
            }
            stmt->type = ASSIGNSTMT;
            return stmt;
        }
        case WRITELN:
        {
            struct ParseWriteLnStmt *writeLnStmt = WriteLnStmt(input, line);
            stmt->writeLnStmt = writeLnStmt;
            if (!writeLnStmt->status)
            {
                ParseError(line, "Incorrect writeln Statement");
                stmt->type = INVALID;
                return stmt;
            }
            stmt->type = WRITELNSTMT;
            return stmt;
        }
        case IDENT:
            ParseError(line, "Invalid variable name/type");
            *stmt = (struct ParseStmt) {INVALID, {NULL}};
            break;
        default:
            PushBackToken(lex);
            *stmt = (struct ParseStmt) {TOKEN, {NULL}};
            return stmt;        
    }
    return stmt;
}

//WritelnStmt ::= writeln (ExprList)
struct ParseWriteLnStmt *WriteLnStmt(FILE* input, int* line)
{
    struct ParseWriteLnStmt temp, *writeStmt = &temp;
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token != LPAREN)
    {
       ParseError(line, "Missing left parenthesis for writeln statement");
       *writeStmt = (struct ParseWriteLnStmt) {0, NULL};
       return writeStmt; 
    }
    struct ParseExprList *exprList = ExprList(input, line);
    if (!exprList->status)
    {
        ParseError(line, "Invalid expression list");
        *writeStmt = (struct ParseWriteLnStmt) {0, exprList};
        return writeStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RPAREN)
    {
        ParseError(line, "Missing right parenthesis for writeln statement");
        *writeStmt = (struct ParseWriteLnStmt) {0, exprList};
        return writeStmt;
    }
    *writeStmt = (struct ParseWriteLnStmt) {1, exprList};
    return writeStmt;
}

//IfStmt ::= if (Expr) '{' StmtList '}' [ else '{' StmtList '}' ] 
struct ParseIfStmt *IfStmt(FILE* input, int* line)
{
    struct ParseIfStmt temp, *IfStmt = &temp;
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token != LPAREN)
    {
        ParseError(line, "Missing left parenthesis for If-Statement");
        *IfStmt = (struct ParseIfStmt) {0, NULL, NULL, NULL};
        return IfStmt;
    }
    struct ParseExpr *condition = Expr(input, line);
    if (!condition->status)
    {
        ParseError(line, "Invalid expression for evaluation in If-Statement");
        *IfStmt = (struct ParseIfStmt) {0, condition, NULL, NULL};
        return IfStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RPAREN)
    {
        ParseError(line, "Missing right parenthesis for If-Statement");
        *IfStmt = (struct ParseIfStmt) {0, condition, NULL, NULL};
        return IfStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != LBRACES)
    {
        ParseError(line, "Missing left brace of If-Clause");
        *IfStmt = (struct ParseIfStmt) {0, condition, NULL, NULL};
        return IfStmt;
    }
    struct ParseStmtList *trueBody = StmtList(input, line);
    if (!trueBody->status)
    {
        ParseError(line, "Invalid statement list in If-Clause");
        *IfStmt = (struct ParseIfStmt) {0, condition, trueBody, NULL};
        return IfStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RBRACES)
    {
        ParseError(line, "Missing right brace of If-Clause");
        *IfStmt = (struct ParseIfStmt) {0, condition, trueBody, NULL};
        return IfStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != ELSE)
    {
        PushBackToken(lex);
        *IfStmt = (struct ParseIfStmt) {1, condition, trueBody, NULL};
        return IfStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != LBRACES)
    {
        ParseError(line, "Missing left brace of Else-Clause");
        *IfStmt = (struct ParseIfStmt) {0, condition, trueBody, NULL};
        return IfStmt;
    }
    struct ParseStmtList *falseBody = StmtList(input, line);
    if (!falseBody->status)
    {
        ParseError(line, "Invalid statement list for Else-Clause");
        *IfStmt = (struct ParseIfStmt) {0, condition, trueBody, falseBody};
        return IfStmt;
    }
    lex = GetNextToken(input, line);
    if (lex->token != RBRACES)
    {
        ParseError(line, "Missing right brace of Else-Clause");
        *IfStmt = (struct ParseIfStmt) {0, condition, trueBody, falseBody};
        return IfStmt;
    }
    *IfStmt = (struct ParseIfStmt) {1, condition, trueBody, falseBody};
    return IfStmt;
}

//AssignStmt ::= Var = Expr
struct ParseAssignStmt *AssignStmt(FILE* input, int* line)
{
    struct ParseAssignStmt temp, *assignStmt = &temp;
    struct ParseVar *var = Var(input, line);
    if (!var->status)
    {
        *assignStmt = (struct ParseAssignStmt) {0, var, NULL};
        return assignStmt;
    }
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token != ASSOP)
    {
        ParseError(line, "Missing assignment operator");
        *assignStmt = (struct ParseAssignStmt) {0, var, NULL};
        return assignStmt;
    }
    struct ParseExpr *expr = Expr(input, line);
    if (!expr->status)
    {
        *assignStmt = (struct ParseAssignStmt) {0, var, expr};
        return assignStmt;
    }
    var->initialized = 1;
    *assignStmt = (struct ParseAssignStmt) {1, var, expr};
    return assignStmt;
}

//Var ::= NIDENT | SIDENT
struct ParseVar *Var(FILE* input, int* line)
{
    short int check = 0; //for determining if var is found in var table
    struct LexItem* lex = GetNextToken(input, line);
    struct ParseVar temp, *var = &temp;
    //struct ParseVar *var = malloc(sizeof(struct ParseVar));
    if (lex->token == NIDENT || lex->token == SIDENT)
    {
        for (int i = 0; i < varIndex; i++)
        {
            if (strcmp(lex->lexeme, variables[i].var.lexeme) == 0)
            {
                if (variables[i].initialized == 0)
                {
                    ParseError(line, "Attempted to use uninitialized variable");
                    *var = (struct ParseVar) {0, 0,*lex};
                    return var;
                }
                check = 1;
                break;
            }
        }
        if (!check)
        {
            variables[varIndex++] = *var = (struct ParseVar) {1, 0, *lex};
            // split up the struct into separate enum Token and lexeme
            return var;
        }
        *var = (struct ParseVar) {1, 0,*lex};
        return var;
    }
    ParseError(line, "Invalid variable name");
    *var = (struct ParseVar) {0, 0,*lex};
    return var;
}

//ExprList:= Expr {,Expr}
struct ParseExprList *ExprList(FILE* input, int* line)
{
    struct ParseExpr *exprArr[10];
    size_t arrIndex = 0;
    struct ParseExprList temp, *exprList = &temp;
    struct LexItem* lex;

    while ((exprArr[arrIndex++] = Expr(input, line)))
    {
        lex = GetNextToken(input, line);
        if (lex->token != COMMA)
        {
            exprArr[arrIndex] = NULL;
            PushBackToken(lex);
            *exprList = (struct ParseExprList) {1, exprArr};
            return exprList;
        }
    }
    exprArr[arrIndex] = NULL;
    ParseError(line, "Missing Expression");
    *exprList = (struct ParseExprList) {0, exprArr};
    return 0;
}

//Expr ::= RelExpr [(-eq|==) RelExpr ]
struct ParseExpr *Expr(FILE* input, int* line)
{
    struct ParseExpr temp, *expr = &temp;
    struct ParseRelExpr *relExpr1 = RelExpr(input, line), *relExpr2;
    if (!relExpr1->status)
    {
        *expr = (struct ParseExpr) {0, relExpr1, NULL, DONE};
        return expr;
    }
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token == NEQ || lex->token == SEQ)
    {
        relExpr2 = RelExpr(input, line);
        if (!relExpr2->status)
        {
            ParseError(line, "Missing operand after operator");
            *expr = (struct ParseExpr) {0, relExpr1, relExpr2, lex->token};
            return expr;
        }
        *expr = (struct ParseExpr) {1, relExpr1, relExpr2, lex->token};
        return expr;
    }
    PushBackToken(lex);
    *expr = (struct ParseExpr) {1, relExpr1, NULL, DONE};
    return expr;
}

//RelExpr ::= AddExpr [ ( -lt | -gt | < | > )  AddExpr ]
struct ParseRelExpr *RelExpr(FILE* input, int* line)
{
    struct ParseRelExpr temp, *relExpr = &temp;
    struct ParseAddExpr *addExpr1 = AddExpr(input, line), *addExpr2;
    if (!addExpr1->status)
    {
        *relExpr = (struct ParseRelExpr) {0, addExpr1, NULL, ERR};
        return relExpr;
    }
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token == NLTHAN || lex->token == NGTHAN 
    || lex->token == SLTHAN || lex->token == SGTHAN)
    {
        addExpr2 = AddExpr(input, line);
        if (!addExpr2->status)
        {
            ParseError(line, "Missing operand after operator");
            *relExpr = (struct ParseRelExpr) {0, addExpr1, addExpr2, lex->token};
            return relExpr;
        }
        *relExpr = (struct ParseRelExpr) {1, addExpr1, addExpr2, lex->token};
        return relExpr;
    }
    PushBackToken(lex);
    *relExpr = (struct ParseRelExpr) {1, addExpr1, NULL, DONE};
    return relExpr;
}

//AddExpr ::= MultExpr { ( + | - | .) MultExpr }
struct ParseAddExpr *AddExpr(FILE* input, int* line)
{
    struct ParseAddExpr temp, *addExpr = &temp; //pointer to return
    struct ParseMultExpr *multExprArr[10]; //array of MultExpr's
    size_t multIndex = 0; //keeps track of highest index for multExprArr
    enum Token tokArray[10];  //array of operators
    size_t tokIndex = 0; //keeps track of highest index for tokArray

    multExprArr[multIndex] = MultExpr(input, line);
    if (!multExprArr[multIndex++]->status)
    {
        multExprArr[multIndex] = NULL;
        *addExpr = (struct ParseAddExpr) {0, multExprArr, NULL};    
        return addExpr;
    }
    struct LexItem* lex = GetNextToken(input, line);
    while (lex->token == PLUS || lex->token == MINUS || lex->token == CAT)
    {
        tokArray[tokIndex++] = lex->token;
        multExprArr[multIndex] = MultExpr(input, line);
        if (!multExprArr[multIndex++]->status)
        {
            ParseError(line, "Missing operand after operator");
            tokArray[tokIndex] = DONE;
            multExprArr[multIndex] = NULL;
            *addExpr = (struct ParseAddExpr) {0, multExprArr, tokArray};
            return addExpr;
        }
        lex = GetNextToken(input, line);
    }
    PushBackToken(lex);
    tokArray[tokIndex] = DONE;
    multExprArr[multIndex] = NULL;
    *addExpr = (struct ParseAddExpr) {1, multExprArr, tokArray};    
    return addExpr;
}

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
struct ParseMultExpr *MultExpr(FILE* input, int* line)
{
    struct ParseMultExpr temp, *multExpr = &temp; //pointer to return
    struct ParseExponExpr *exponExprArr[10]; //array of ExponExpr's
    size_t exponIndex = 0; //keeps track of highest index for exponExprArr
    enum Token tokArray[10]; //array of operators
    size_t tokIndex = 0; //keeps track of highest index for tokArray

    exponExprArr[exponIndex] = ExponExpr(input, line);
    if (!exponExprArr[exponIndex]->status)
    {
        exponExprArr[exponIndex] = NULL;
        *multExpr = (struct ParseMultExpr) {0, exponExprArr, NULL};    
        return multExpr;
    }
    struct LexItem* lex = GetNextToken(input, line);
    while (lex->token == MULT || lex->token == DIV || lex->token == SREPEAT)
    {
        tokArray[tokIndex++] = lex->token;
        exponExprArr[exponIndex] = ExponExpr(input, line);
        if (!exponExprArr[exponIndex++]->status)
        {
            ParseError(line, "Missing operand after operator");
            tokArray[tokIndex] = DONE;
            exponExprArr[exponIndex] = NULL;
            *multExpr = (struct ParseMultExpr) {0, exponExprArr, tokArray};
            return multExpr;
        }
        lex = GetNextToken(input, line);
    }
    PushBackToken(lex);
    tokArray[tokIndex] = DONE;
    exponExprArr[exponIndex] = NULL;
    *multExpr = (struct ParseMultExpr) {1, exponExprArr, tokArray};    
    return multExpr;
}

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
struct ParseExponExpr *ExponExpr(FILE* input, int* line)
{
    struct ParseExponExpr temp, *exponExpr = &temp; //pointer to return
    struct ParseUnaryExpr *unaryExprArr[10]; //array of unaryExpr's
    size_t unaryIndex = 0; //keeps track of highest index for unaryExprArr
    enum Token tokArray[10]; //array of operators
    size_t tokIndex = 0; //keeps track of highest index for tokArray

    unaryExprArr[unaryIndex] = UnaryExpr(input, line);
    if (!unaryExprArr[unaryIndex]->status)
    {
        unaryExprArr[unaryIndex] = NULL;
        *exponExpr = (struct ParseExponExpr) {0, unaryExprArr, NULL};    
        return exponExpr;
    }
    struct LexItem* lex = GetNextToken(input, line);
    while (lex->token == EXPONENT)
    {
        tokArray[tokIndex++] = lex->token;
        unaryExprArr[unaryIndex] = UnaryExpr(input, line);
        if (!unaryExprArr[unaryIndex++]->status)
        {
            ParseError(line, "Missing operand after operator");
            tokArray[tokIndex] = DONE;
            unaryExprArr[unaryIndex] = NULL;
            *exponExpr = (struct ParseExponExpr) {0, unaryExprArr, tokArray};
            return exponExpr;
        }
        lex = GetNextToken(input, line);
    }
    PushBackToken(lex);
    tokArray[tokIndex] = DONE;
    unaryExprArr[unaryIndex] = NULL;
    *exponExpr = (struct ParseExponExpr) {1, unaryExprArr, tokArray};
    return exponExpr;
}

//UnaryExpr ::= [( - | + )] PrimaryExpr
struct ParseUnaryExpr *UnaryExpr(FILE* input, int* line)
{
    struct ParseUnaryExpr temp, *unaryExpr = &temp;
    int sign = 0;
    struct LexItem* lex = GetNextToken(input, line);
    if (lex->token == PLUS)
        sign = 1;
    else if (lex->token == MINUS)
        sign = -1;
    else 
        PushBackToken(lex);
    struct ParsePrimaryExpr *primaryExpr = PrimaryExpr(input, line);
    *unaryExpr = (struct ParseUnaryExpr) {primaryExpr->status, primaryExpr, sign};
    return unaryExpr;
}

//PrimaryExpr ::= IDENT | NIDENT | SIDENT | ICONST | RCONST | SCONST | (Expr)
struct ParsePrimaryExpr *PrimaryExpr(FILE* input, int* line)
{
    struct ParsePrimaryExpr temp, *primaryExpr = &temp;
    primaryExpr->status = 1;
    struct ParseExpr *expr; 
    struct LexItem* lex = GetNextToken(input, line);
    switch (lex->token)
    {
        case ICONST:
            primaryExpr->integer = atoi(lex->lexeme);
            return primaryExpr;
        case RCONST:
            primaryExpr->real = atof(lex->lexeme);
            return primaryExpr;
        case SCONST:
            primaryExpr->string = lex->lexeme;
            return primaryExpr;
        case IDENT: case NIDENT: case SIDENT:
            for (int i = 0; i < varIndex; i++)
            {
                if (strcmp(lex->lexeme, variables[i].var.lexeme) == 0)
                {
                    if (variables[i].initialized == 0)
                    {
                        ParseError(line, "Attempted to use undefined variable");
                        return 0;
                    }
                }
            }
            primaryExpr->ident = *lex;
            return primaryExpr;
        case LPAREN:
            expr = Expr(input, line);
            primaryExpr->expr = expr;
            if(!expr)
            {
                ParseError(line, "Missing expression after left parenthesis of expression");
                return 0;
            }
            lex = GetNextToken(input, line);
            if (lex->token != RPAREN)
            {
                ParseError(line, "Missing closing parenthesis of expression");
                return 0;
            }
            return primaryExpr;
        default:
            *primaryExpr = (struct ParsePrimaryExpr) {0};
            return primaryExpr;
    }
}