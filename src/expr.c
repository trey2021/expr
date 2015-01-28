#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack.h"
#include "shunting_yard.h"
#include "eval.h"

#define EXIT_INVALID_EXPR 2
#define EXIT_ZERO 1
#define EXIT_NONZERO 0

int main(int argc, char *argv[])
{
    int exitCode = EXIT_INVALID_EXPR;
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
			exitCode = 0 == result ? EXIT_ZERO : EXIT_NONZERO;

            printf("%i\n", result);
        }
    }

    stack_free(&stack);
    queue_free(&queue);

    return exitCode;
}
