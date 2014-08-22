#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack.h"
#include "shunting_yard.h"

static int evaluate_expr(ExprQueue *pInQueue, int *pResult);

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

                pNewElement->value.intValue = pElement->value.pOpValue->
                    func(pEA->value.intValue, pEB->value.intValue);
                pNewElement->type = INT;

                free(pEA);
                free(pEB);

                stack_push(&stack, pNewElement);
                break;
            }
            default:
                fprintf(stderr, "Wtf did you do? Found %s on stack.\n", 
                    pElement->value.pStrValue);
                error = 1;
                break;
        }
    }

    *pResult = stack_pop(&stack)->value.intValue;
    
    if (!stack_isempty(&stack))
    {
        fprintf(stderr, "Invalid expression; extraneous numbers in expression.\n");        
        error = 1;
    }

    return !error; 
}

int main(int argc, char *argv[])
{
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
