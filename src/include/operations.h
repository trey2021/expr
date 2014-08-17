#ifndef H_OPERATIONS_H
#define H_OPERATIONS_H

#include "expr_types.h"

ExprOperation *find_operation(char *pValue, ExprOperation *pOperations);

int operation_add(int a, int b);
int operation_sub(int a, int b);
int operation_div(int a, int b);
int operation_mul(int a, int b);
int operation_mod(int a, int b);
int operation_pow(int a, int b);

#endif
