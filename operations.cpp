#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"
#include <cstring>

int operations (char *operation, Stack *stack)
{
    scanf ("%s", operation);

    if (strcmp (operation, "PUSH") == 0)
    {
        int arg = 0;
        scanf ("%d", &arg);
        stackPush (stack, arg, VALUES_FOR_ERROR);
        return 1;

    } else if (strcmp (operation, "POP") == 0)
    {
        stackPop (stack, VALUES_FOR_ERROR);
        return 2;

    } else if (strcmp (operation, "ADD") == 0)
    {
        int term = stackPop (stack, VALUES_FOR_ERROR);
        int result = stackPop (stack, VALUES_FOR_ERROR) + term;
        stackPush (stack, result, VALUES_FOR_ERROR);
        return 3;

    } else if (strcmp (operation, "SUB") == 0)
    {
        int subtrahend = stackPop (stack, VALUES_FOR_ERROR);
        int result = stackPop (stack, VALUES_FOR_ERROR) - subtrahend;
        stackPush (stack, result, VALUES_FOR_ERROR);
        return 4;

    } else if (strcmp (operation, "MUL") == 0)
    {
        int result = stackPop (stack, VALUES_FOR_ERROR) * stackPop (stack, VALUES_FOR_ERROR);
        stackPush (stack, result, VALUES_FOR_ERROR);
        return 5;

    } else if (strcmp (operation, "DIV") == 0)
    {
        int denominator = stackPop (stack, VALUES_FOR_ERROR);
        int result = stackPop (stack, VALUES_FOR_ERROR) / denominator;
        stackPush (stack, result, VALUES_FOR_ERROR);
        return 6;

    } else if (strcmp (operation, "OUT") == 0)
    {
        int out = stackPop (stack, VALUES_FOR_ERROR);
        printf ("\n%d\n\n", out);
        return 7;

    } else if (strcmp (operation, "HLT") == 0)
    {
        return -1;

    } else {

        printf ("Syntax error\n");
        return -2;
    }
}