#include <stdlib.h>
#include "queue.h"
#include <stdio.h>

void queue_init(ExprQueue *pQueue)
{
    pQueue->pHead = malloc(sizeof(ExprQueueNode));
    pQueue->pHead->pNext = NULL;
}

void queue_enqueue(ExprQueue *pQueue, ExprElement *pElement)
{
    ExprQueueNode *pOldHead = pQueue->pHead->pNext;
    
    pQueue->pHead->pNext = malloc(sizeof(ExprQueueNode));
    pQueue->pHead->pNext->pElement = pElement;
    pQueue->pHead->pNext->pNext = pOldHead;
}

ExprElement *queue_dequeue(ExprQueue *pQueue)
{
    ExprQueueNode *pEnd = pQueue->pHead;
    ExprElement *pResult = NULL; 

    if (pEnd != NULL && pEnd->pNext != NULL)
    {
        while (pEnd->pNext->pNext != NULL)
        {
            pEnd = pEnd->pNext;
        }
        
        pResult = pEnd->pNext->pElement;
        free(pEnd->pNext);
        pEnd->pNext = NULL;
    }

    return pResult;
}

int queue_isempty(ExprQueue *pQueue)
{
    return pQueue->pHead->pNext == NULL;
}

void queue_clear(ExprQueue *pQueue)
{
    while (!queue_isempty(pQueue))
    {
        queue_dequeue(pQueue);
    }
}

void queue_free(ExprQueue *pQueue)
{
	queue_clear(pQueue);
	free(pQueue->pHead);
}
