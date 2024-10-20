#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"
#include <cstring>

CMD operations (Stack *stack)
{
    int cmd = 0; 
    scanf ("%d", &cmd);

    if (cmd == 1)
    {
        int arg = 0;
        scanf ("%d", &arg);
        stackPush (stack, arg, VALUES_FOR_ERROR);
        return PUSH;

    }/* else if (cmd == POP)
    {
        stackPop (stack, VALUES_FOR_ERROR);
        return POP;

    } else if (cmd == ADD)
    {
        int term = stackPop (stack, VALUES_FOR_ERROR);
        int result = stackPop (stack, VALUES_FOR_ERROR) + term;
        stackPush (stack, result, VALUES_FOR_ERROR);
        return ADD;

    } else if (cmd == SUB)
    {
        int subtrahend = stackPop (stack, VALUES_FOR_ERROR);
        int result = stackPop (stack, VALUES_FOR_ERROR) - subtrahend;
        stackPush (stack, result, VALUES_FOR_ERROR);
        return SUB;

    } else if (cmd == MUL)
    {
        int result = stackPop (stack, VALUES_FOR_ERROR) * stackPop (stack, VALUES_FOR_ERROR);
        stackPush (stack, result, VALUES_FOR_ERROR);
        return MUL;

    } else if (cmd == DIV)
    {
        int denominator = stackPop (stack, VALUES_FOR_ERROR);
        int result = stackPop (stack, VALUES_FOR_ERROR) / denominator;
        stackPush (stack, result, VALUES_FOR_ERROR);
        return DIV;

    } else if (cmd == OUT)
    {
        int out = stackPop (stack, VALUES_FOR_ERROR);
        printf ("\n%d\n\n", out);
        return OUT;

    } else if (cmd == HLT)
    {
        return HLT;

    } */else {

        printf ("Syntax error\n");
        return ERROR;
    }
}