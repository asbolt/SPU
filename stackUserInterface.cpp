#include "stackInsideFunctoins.h"
#include "stackUserInterface.h"

StackErrors stackPush (Stack *stack, stackElementType element, int line, const char* function, const char* file)
{
    STACK_CHECK(stack, line, function, file);

    if (element == stack->poisonValue)
    {
        printf ("bad value\n");
        return STACK_BAD_ELEMENT;
    }

    if (stack->size >= stack->capacity - SPACE_FOR_CANARIES)
    {
        stackExtend (stack, line, function, file);
        GIVE_VALUES_FOR_CANARIES(stack);
    }

    stack->data[stack->size + 1] = element;
    (stack->size)++;

    stack->hash = dataHash ((stack->data), stack->size);

    STACK_CHECK(stack, line, function, file);

    return STACK_GOOD;
}

int stackPop (Stack *stack, int line, const char* function, const char* file)
{
    STACK_CHECK(stack, line, function, file);

    int value = stack->data[stack->size];
    stack->data[stack->size] = stack->poisonValue;
    (stack->size)--;

    if (stack->size + SPACE_FOR_CANARIES < (stack->capacity)/4)
    {
        stackReduce (stack, line, function, file);
        GIVE_VALUES_FOR_CANARIES(stack);
    }

    stack->hash = dataHash ((stack->data), stack->size);

    STACK_CHECK(stack, line, function, file);

    return value;
}

StackErrors stackCtor (Stack *stack, stackElementType size)
{
    stack->data = (stackElementType*)calloc(size + SPACE_FOR_CANARIES, sizeof(stackElementType));
    if (stack->data == NULL)
        return STACK_DATA_NULL;

    stack->capacity = size;

    GIVE_VALUES_FOR_CANARIES(stack);

    stack->hash = dataHash ((stack->data), stack->size);
    stack->poisonValue = POISON_VALUE;
    stack->startCanary = CANARY_VALUE;
    stack->endCanary = CANARY_VALUE;
    stack->stackError = STACK_GOOD;

    return STACK_GOOD;
}

StackErrors stackDtor (Stack *stack, int line, const char* function, const char* file)
{
    STACK_CHECK(stack, line, function, file);

    free (stack->data);
    stack->data = NULL;

    return STACK_GOOD;
}

StackErrors stackDump (Stack *stack)
{
    if (stack->data == NULL)
    {
        printf ("Address data: NULL");
    } else {
        printf ("Address data: %p\n", stack->data);
    }

    printf ("Data: ");
    for (int i = 0; i < stack->size; i++)
    {
        printf ("%d ", stack->data[i + 1]);
    }
    printf ("\n");

    printf ("Size: %d\n", stack->size);
    printf ("Capacity: %d\n", stack->capacity);
    printf ("Poison value: %d\n", stack->poisonValue);
    printf ("Hash: %d\n", stack->hash);

    printf ("\n\n\n");

    return STACK_GOOD;
}
