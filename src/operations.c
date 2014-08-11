#include <stdlib.h>
#include <math.h>
#include "operations.h"

ExprOperation *find_operation(char *pValue, ExprOperation *pOperations)
{
    int i = 0;

    for(i = 0; pOperations[i].operation != 0; i++) 
    {
        if (pOperations[i].operation == *pValue)
        {
            return &pOperations[i];
        }
    }

    return NULL;
}

int operation_add(int a, int b)
{
    return a + b;
}

int operation_sub(int a, int b)
{
    return a - b;
}

int operation_div(int a, int b)
{
    return a / b;
}

int operation_mul(int a, int b)
{
    return a * b;
}

int operation_mod(int a, int b)
{
    return a % b;
}

int operation_pow(int a, int b)
{
    return (int)pow((double)a, (double)b);
}

