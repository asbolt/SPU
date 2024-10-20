#include "stackError.h"
#include "stack.h"

StackErrors stackCheckForError (Stack *stack)
{
    if (stack == NULL)
    {
        return STACK_NULL;
    } else if (stack->data[0] != CANARY_VALUE || stack->data[stack->capacity - 1] != CANARY_VALUE) {
        return STACK_BAD_CANARY;

    } else if (stack->data == NULL)
    {
        return STACK_DATA_NULL;

    } else if (stack->size < 0)
    {
        return STACK_BAD_SIZE;

    } else if (stack->capacity < 0)
    {
        return STACK_BAD_CAPACITY;
    } else 
    if (stack->size > stack->capacity)
    {
        return STACK_CAPACITY_LESS_THEN_SIZE;
    }

    for (int i = 0; i < stack->size; i++)
    {
        if (stack->data[i+1] == stack->poisonValue)
        {
            return STACK_BAD_ELEMENT;
        }
    }

    return STACK_GOOD;
}

StackErrors stackPrintError (Stack *stack, int line, const char* function, const char* file)
{
    switch (stack->stackError)
    {
    case STACK_NULL:
        printf ("Stack = NULL\n");
        break;

    case STACK_DATA_NULL:
        printf ("Data = NULL\n");
        break;

    case STACK_BAD_SIZE:
        printf ("Size < 0\n");
        break;

    case STACK_BAD_CAPACITY:
        printf ("Capacity < 0\n");
        break;

    case STACK_BAD_ELEMENT:
        printf ("Element = poison value\n");
        break;

    case STACK_BAD_CANARY:
        printf ("Data corrupted\n");
        break;

    case STACK_CAPACITY_LESS_THEN_SIZE:
        printf ("Capacity < size\n");
        break;

    case STACK_GOOD:
        break;
    
    default:
        break;
    }

    printf ("Error in file %s:%d (function %s)\n", file, line, function);
    printf ("%d", stack->stackError);

    return stack->stackError;
}
