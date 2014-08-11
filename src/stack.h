#ifndef H_STACK_H
#define H_STACK_H

#include "expr_types.h"

#define EXPR_STACK_DEFAULT_CAP 16

typedef struct
{
    ExprElement **pContents;
    int cap;
    int size;
} ExprStack;

void stack_init(ExprStack *pStack);
void stack_push(ExprStack *pStack, ExprElement *pElement);
ExprElement *stack_pop(ExprStack *pStack);
ExprElement *stack_peek(ExprStack *pStack);
int stack_isempty(ExprStack *pStack);
void stack_clear(ExprStack *pStack);
void stack_free(ExprStack *pStack);

#endif
