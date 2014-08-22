#ifndef H_SHUNTING_YARD_H
#define H_SHUNTING_YARD_H

#include "queue.h"
#include "operations.h"

int infix_to_postfix(int tokensCount, char *tokens[], 
    ExprQueue *pOutQueue, ExprOperation *pOperations);

#endif
