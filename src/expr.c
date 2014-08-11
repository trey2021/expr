#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack.h"
#include "queue.h"
#include "operations.h"

static int is_number(char *pValue);
static int create_expr_element(ExprElement *pElement, char *pValue, ExprOperation *pOperations);
static int is_lower_precedence(ExprOperation *pOpA, ExprOperation *pOpB);
static int infix_to_postfix(int argc, char *argv[], ExprQueue *pOutQueue, ExprOperation *pOperations);
static int evaluate_expr(ExprQueue *pInQueue, int *pResult);

static int is_number(char *pValue)
{
    char *pValuePtr = pValue;
    int foundPeriod = 0;
   
    for(; isdigit(*pValuePtr) || (!foundPeriod && (foundPeriod = *pValuePtr == '.')); pValuePtr++)
        ;

    return *pValuePtr == '\0';
}

static int create_expr_element(ExprElement *pElement, char *pValue, ExprOperation *pOperations)
{
    int type = 0;

    if (is_number(pValue))
    {
        type = INT;
        pElement->value.value = atoi(pValue);
    }
    else
    {
        ExprOperation *pOp = find_operation(pValue, pOperations);

        if (NULL != pOp)
        {
            type = OPERATION;
            pElement->value.pOperation = pOp;
        }
        else
        {
            pElement->value.pValue = pValue;

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

static int infix_to_postfix(int tokensCount, char *tokens[], ExprQueue *pOutQueue, ExprOperation *pOperations)
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
            {
                ExprElement *pPeekElm = NULL;
                for (pPeekElm = stack_peek(&stack);
                    pPeekElm != NULL && pPeekElm->type != OPEN_PAREN; 
                    pPeekElm = stack_peek(&stack))
                {
                    queue_enqueue(pOutQueue, stack_pop(&stack));
                }

                stack_pop(&stack);
                break;
            }
            case OPERATION:
            {
                ExprOperation *pOperation = pElement->value.pOperation; 
                ExprElement *pPeekElm = NULL;

                for (pPeekElm = stack_peek(&stack);
                    (pPeekElm != NULL && pPeekElm->type == OPERATION)
                        && is_lower_precedence(pOperation, pPeekElm->value.pOperation);
                    pPeekElm = stack_peek(&stack))
                {
                    queue_enqueue(pOutQueue, stack_pop(&stack));
                }

                stack_push(&stack, pElement);
                break;
            }
            default:
                fprintf(stderr, "Error: token \'%s\' not a number or operation.\n", tokens[i]);
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

static int evaluate_expr(ExprQueue *pInQueue, int *pResult)
{
    int error = 0;
    ExprStack stack;

    stack_init(&stack);
    
    while (!queue_isempty(pInQueue))
    {
        ExprElement *pElement = queue_dequeue(pInQueue);

        switch(pElement->type)
        {
            case INT:
                stack_push(&stack, pElement);
                break;
            case OPERATION:
            {                
                ExprElement *pNewElement = malloc(sizeof(ExprElement));
                ExprElement *pEB = stack_pop(&stack);
                ExprElement *pEA = stack_pop(&stack);
                int b = pEB->value.value;
                int a = pEA->value.value;

                free(pEA);
                free(pEB);

                pNewElement->value.value = pElement->value.pOperation->func(a, b);
                pNewElement->type = INT;

                stack_push(&stack, pNewElement);
                break;
            }
            default:
                fprintf(stderr, "Wtf did you do? Found %s on stack.\n", pElement->value.pValue);
                error = 1;
                break;
        }
    }

    *pResult = stack_pop(&stack)->value.value;
    
    if (!stack_isempty(&stack))
    {
        fprintf(stderr, "Invalid expression; extraneous numbers in expression.\n");        
        error = 1;
    }

    return !error; 
}

int main(int argc, char *argv[])
{
    int i = 0;
    int exitCode = EXIT_FAILURE;
    ExprStack stack;
    ExprQueue queue;
    ExprOperation operations[] = 
    {
        {'-', 2, 1, &operation_sub},
        {'+', 2, 1, &operation_add},
        {'*', 3, 1, &operation_mul},
        {'/', 3, 1, &operation_div},
        {'%', 3, 1, &operation_mod},
        {'^', 4, 0, &operation_pow},
        {0}
    };
       
    stack_init(&stack);
    queue_init(&queue);

    if (infix_to_postfix(argc, argv, &queue, operations))
    {
        int result = 0;

        if (evaluate_expr(&queue, &result))
        {
            exitCode = EXIT_SUCCESS;
            printf("%i\n", result);
        }
    }

	stack_free(&stack);
	queue_free(&queue);

    return exitCode;
}
