#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "queue.h"


int evaluate_expr(ExprQueue *pInQueue, int *pResult)
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
                fprintf(stderr, "ERROR: Found %s on stack.\n", 
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
