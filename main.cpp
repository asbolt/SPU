#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"


int main ()
{
    char operation [10] = {}; 

    Stack *stack;
    stackCtor (stack, 10);

    while (1)
    {
        if (operations (operation, stack) < 0)
            return 0;
    }

    return 0;
}