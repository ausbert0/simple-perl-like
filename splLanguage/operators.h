#ifndef OPERATORS_H_
#define OPERATORS_H_

#include "interpreter.h"

void InterpreterError(char *);

//Expr ::= RelExpr [(-eq|==) RelExpr]
struct Value *neq(struct Value *left, struct Value *right);
struct Value *seq(struct Value *left, struct Value *right);

//RelExpr ::= AddExpr [ ( -lt | -gt | < | > )  AddExpr ]
struct Value *slthan(struct Value *left, struct Value *right);
struct Value *sgthan(struct Value *left, struct Value *right);
struct Value *nlthan(struct Value *left, struct Value *right);
struct Value *ngthan(struct Value *left, struct Value *right);

//AddExpr ::= MultExpr { ( + | - | .) MultExpr }
struct Value *plus(struct Value *left, struct Value *right);
struct Value *minus(struct Value *left, struct Value *right);
struct Value *cat(struct Value *left, struct Value *right);

//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
struct Value *mult(struct Value *left, struct Value *right);
struct Value *divide(struct Value *left, struct Value *right);
struct Value *srepeat(struct Value *left, struct Value *right);

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
struct Value *exponent(struct Value *left, struct Value *right);

//UnaryExpr ::= [( - | + )] PrimaryExpr
struct Value *signedExpr(struct Value *value, short unsigned int sign);

// struct Value *exponent(struct Value *left, struct Value *right);

#endif