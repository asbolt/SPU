#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"


int main ()
{
    Stack *stack;
    stackCtor (stack, 10);

    while (1)
    {
        if (operations (stack) < 0)
            return 0;
    }

    stackDtor (stack, VALUES_FOR_ERROR);

    return 0;
}