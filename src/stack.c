#include <stdlib.h>
#include "stack.h"

static void stack_resize(ExprStack *pStack)
{
    pStack->cap *= 2;
    pStack->pContents = realloc(pStack->pContents, sizeof(ExprElement*) * pStack->cap);
}

void stack_init(ExprStack *pStack)
{
    pStack->cap = EXPR_STACK_DEFAULT_CAP;
    pStack->size = 0;
    pStack->pContents = malloc(sizeof(ExprElement*) * pStack->cap);
}

void stack_push(ExprStack *pStack, ExprElement *pElement)
{
    if (pStack->size == pStack->cap)
    {
        stack_resize(pStack);
    }
    pStack->pContents[pStack->size++] = pElement;
}

ExprElement *stack_pop(ExprStack *pStack)
{
    return pStack->size > 0 ? pStack->pContents[--pStack->size] : NULL;
}

ExprElement *stack_peek(ExprStack *pStack)
{
    return pStack->size > 0 ? pStack->pContents[pStack->size - 1] : NULL;
}

int stack_isempty(ExprStack *pStack)
{
    return pStack->size == 0;
}

void stack_clear(ExprStack *pStack)
{
	pStack->size = 0;
}

void stack_free(ExprStack *pStack)
{
	free(pStack->pContents);
}

