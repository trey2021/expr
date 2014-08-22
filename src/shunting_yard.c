#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "expr_types.h"
#include "stack.h"
#include "shunting_yard.h"

static int is_number(char *pValue);
static int create_expr_element(ExprElement *pElement, char *pValue, 
    ExprOperation *pOperations);
static int is_lower_precedence(ExprOperation *pOpA, ExprOperation *pOpB);
static void pop_nested_expr(ExprQueue *pQueue, ExprStack *pStack);
static void pop_lower_order_operations(ExprQueue *pQueue, ExprOperation *pOperation, 
	ExprStack *pStack);

static int is_number(char *pValue)
{
	char *pEndPtr = NULL;

	strtol(pValue, &pEndPtr, 10);

	return *pEndPtr == '\0';
}

static int create_expr_element(ExprElement *pElement, char *pValue, 
    ExprOperation *pOperations)
{
    int type = 0;

    if (is_number(pValue))
    {
        type = INT;
        pElement->value.intValue = atoi(pValue);
    }
    else
    {
        ExprOperation *pOp = find_operation(pValue, pOperations);

        if (NULL != pOp)
        {
            type = OPERATION;
            pElement->value.pOpValue = pOp;
        }
        else
        {
            pElement->value.pStrValue = pValue;

            if (*pValue == '(')
            {
                type = OPEN_PAREN;
            }
            else if (*pValue == ')')
            {
                type = CLOSE_PAREN;
            }
        }
    }

    pElement->type = type;

    return type;
}

static int is_lower_precedence(ExprOperation *pOpA, ExprOperation *pOpB)
{
    return ((pOpA->isLeft && pOpA->precedence <= pOpB->precedence)
        || pOpA->precedence < pOpB->precedence);
}

static void pop_nested_expr(ExprQueue *pQueue, ExprStack *pStack)
{
	ExprElement *pPeekElm = stack_peek(pStack);

	for (;
		pPeekElm != NULL && pPeekElm->type != OPEN_PAREN; 
		pPeekElm = stack_peek(pStack))
	{
		queue_enqueue(pQueue, stack_pop(pStack));
	}

	stack_pop(pStack); //pop paren
}

static void pop_lower_order_operations(ExprQueue *pQueue, ExprOperation *pOperation, 
	ExprStack *pStack)
{
	ExprElement *pPeekElm = stack_peek(pStack);

	for (;
		pPeekElm != NULL && pPeekElm->type == OPERATION
			&& is_lower_precedence(pOperation, pPeekElm->value.pOpValue);
		pPeekElm = stack_peek(pStack))
	{
		queue_enqueue(pQueue, stack_pop(pStack));
	}
}

int infix_to_postfix(int tokensCount, char *tokens[], 
    ExprQueue *pOutQueue, ExprOperation *pOperations)
{
    int i = 0;
    int error = 0;
    ExprStack stack;
    stack_init(&stack);

    for(i = 1; i < tokensCount && !error; i++)
    {
        ExprElement *pElement = malloc(sizeof(ExprElement));

        switch(create_expr_element(pElement, tokens[i], pOperations))
        {
            case INT:
                queue_enqueue(pOutQueue, pElement);
                break;
            case OPEN_PAREN:
                stack_push(&stack, pElement);
                break;
            case CLOSE_PAREN:
				pop_nested_expr(pOutQueue, &stack);
                break;
            case OPERATION:
            {
				pop_lower_order_operations(pOutQueue, pElement->value.pOpValue, &stack);
				stack_push(&stack, pElement);
                break;
            }
            default:
                fprintf(stderr, "Error: token \'%s\' not a number or operation.\n", 
                    tokens[i]);
                queue_clear(pOutQueue);
                error = 1;
                break;
        }
    }

    while (!stack_isempty(&stack))
    {
        queue_enqueue(pOutQueue, stack_pop(&stack));
    }

    return !error;
}

