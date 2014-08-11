#ifndef H_QUEUE_H
#define H_QUEUE_H

#include "expr_types.h"

#define EXPR_QUEUE_DEFAULT_CAP 16

typedef struct _ExprQueueNode
{
    ExprElement *pElement;
    struct _ExprQueueNode *pNext;
} ExprQueueNode;

typedef struct
{
    ExprQueueNode *pHead;
} ExprQueue;

void queue_init(ExprQueue *pQueue);
void queue_enqueue(ExprQueue *pQueue, ExprElement *pElement);
ExprElement *queue_dequeue(ExprQueue *pQueue);
int queue_isempty(ExprQueue *pQueue);
void queue_clear(ExprQueue *pQueue);

#endif
