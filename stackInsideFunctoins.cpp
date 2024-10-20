#include "stackInsideFunctoins.h"

StackErrors stackExtend (Stack *stack, int line, const char* function, const char* file)
{
    STACK_CHECK(stack, line, function, file);

    stack->data = (stackElementType*)realloc (stack->data, (size_t)((stack->capacity) * (int)sizeof(stackElementType) * EXTEND_NUMBER) + SPACE_FOR_CANARIES);
    if (stack->data == NULL)
        return STACK_DATA_NULL;
    
    stack->capacity = (stack->capacity) * EXTEND_NUMBER;

    for (int i = stack->size; i < stack->capacity - 1; i++)
    {
        stack->data[i+1] = stack->poisonValue;
    }

    return STACK_GOOD;
}

StackErrors stackReduce (Stack *stack, int line, const char* function, const char* file)
{
    STACK_CHECK(stack, line, function, file);

    stack->data = (stackElementType*)realloc (stack->data, (size_t)((stack->capacity) * (int)sizeof(stackElementType) / REDUCE_NUMBER + SPACE_FOR_CANARIES));
    if (stack->data == NULL)
        return STACK_DATA_NULL;

    stack->capacity = (stack->capacity) / REDUCE_NUMBER;

    return STACK_GOOD;
}

long long int dataHash (stackElementType *array, int arraySize)
{
    long long int hash = 0;

    for (int numberElement = 0, degree = 1; numberElement < arraySize - 1; numberElement++, degree = degree * NUMBER_FOR_CALCULATION_HASH)
    {
        hash += (char)array[numberElement + 1]*degree;
    }

    return hash;
}
