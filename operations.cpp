#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"
#include <cstring>

CMD operations (SPU *spu)
{

    if (spu->code[spu->id] == 1)
    {
        (spu->id)++;

        int arg = spu->code[spu->id];
        (spu->id)++;
        stackPush (&spu->stack, arg, VALUES_FOR_ERROR);
        
        return PUSH;

    } else if (spu->code[spu->id] == POP)
    {
        (spu->id)++;

        stackPop (&spu->stack, VALUES_FOR_ERROR);
        return POP;

    } else if (spu->code[spu->id] == ADD)
    {
        (spu->id)++;

        int term = stackPop (&spu->stack, VALUES_FOR_ERROR);
        int result = stackPop (&spu->stack, VALUES_FOR_ERROR) + term;
        stackPush (&spu->stack, result, VALUES_FOR_ERROR);
        return ADD;

    } else if (spu->code[spu->id] == SUB)
    {
        (spu->id)++;

        int subtrahend = stackPop (&spu->stack, VALUES_FOR_ERROR);
        int result = stackPop (&spu->stack, VALUES_FOR_ERROR) - subtrahend;
        stackPush (&spu->stack, result, VALUES_FOR_ERROR);
        return SUB;

    } else if (spu->code[spu->id] == MUL)
    {
        (spu->id)++;

        int result = stackPop (&spu->stack, VALUES_FOR_ERROR) * stackPop (&spu->stack, VALUES_FOR_ERROR);
        stackPush (&spu->stack, result, VALUES_FOR_ERROR);
        return MUL;

    } else if (spu->code[spu->id] == DIV)
    {
        (spu->id)++;

        int denominator = stackPop (&spu->stack, VALUES_FOR_ERROR);
        int result = stackPop (&spu->stack, VALUES_FOR_ERROR) / denominator;
        stackPush (&spu->stack, result, VALUES_FOR_ERROR);
        return DIV;

    } else if (spu->code[spu->id] == OUT)
    {
        (spu->id)++;

        int out = stackPop (&spu->stack, VALUES_FOR_ERROR);
        printf ("\n%d\n\n", out);
        return OUT;

    } else if (spu->code[spu->id] == -1)
    {
        return HLT;

    } else {

        printf ("Syntax error\n");
        return ERROR;
    }
}

CMD assembler (SPU *spu, FILE *ptrFile)
{
    int cmd = 0;
    fscanf (ptrFile, "%d", &cmd);

    if (cmd == PUSH)
    {
        spu->code[spu->codeSize] = PUSH;
        (spu->codeSize)++;

        fscanf (ptrFile, "%d", &spu->code[spu->codeSize]);
        (spu->codeSize)++;

        return PUSH;

    } else if (cmd == POP)
    {
        spu->code[spu->codeSize] = POP;
        (spu->codeSize)++;
        return POP;

    } else if (cmd == ADD)
    {
        spu->code[spu->codeSize] = ADD;
        (spu->codeSize)++;
        return ADD;

    } else if (cmd == SUB)
    {
        spu->code[spu->codeSize] = SUB;
        (spu->codeSize)++;
        return SUB;

    } else if (cmd == MUL)
    {
        spu->code[spu->codeSize] = MUL;
        (spu->codeSize)++;
        return MUL;

    } else if (cmd == DIV)
    {
        spu->code[spu->codeSize] = DIV;
        (spu->codeSize)++;
        return DIV;

    } else if (cmd == OUT)
    {
        spu->code[spu->codeSize] = OUT;
        (spu->codeSize)++;
        return OUT;

    } else if (cmd == HLT)
    {
        spu->code[spu->codeSize] = HLT;
        return HLT;

    } else {

        printf ("Syntax error\n");
        return ERROR;
    }
}

int readFile (SPU *spu)
{
    FILE * ptrFile = fopen (FILE_NAME, "rb");
    if (ptrFile == NULL)
    {
        return 1;
    }

    while (1)
    {
        if (assembler (spu, ptrFile) < 0)
            return 1;
    }
}