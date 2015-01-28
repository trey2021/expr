#ifndef H_EXPR_TYPES_H
#define H_EXPR_TYPES_H

typedef enum
{
	UNRECOGNIZED_TYPE,
    INT,
    OPERATION,
    OPEN_PAREN,
    CLOSE_PAREN
} ElementType;

typedef int (*OperationFunc)(int a, int b);

typedef struct 
{
    char operation;
    unsigned char precedence;
    unsigned char isLeft;
    OperationFunc func;    
} ExprOperation;

typedef struct
{
    union 
    {
        char *pStrValue;
        ExprOperation *pOpValue;
        int intValue;
    } value;
    ElementType type;
} ExprElement;

#endif
