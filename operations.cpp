#include "stack.h"
#include "operations.h"
#include "stackUserInterface.h"
#include <cstring>

int operations (SPU *spu)
{
    int cmd = spu->code[spu->ip];

    switch (cmd)
    {
        case PUSH:
            {(spu->ip)++;

            int arg = spu->code[spu->ip];
            (spu->ip)++;
            stackPush (&spu->stack, arg, VALUES_FOR_ERROR);
            break;}

        case POP:
            {(spu->ip)++;

            stackPop (&spu->stack, VALUES_FOR_ERROR);
            break;}

        case ADD:
            {(spu->ip)++;

            int term = stackPop (&spu->stack, VALUES_FOR_ERROR);
            int result = stackPop (&spu->stack, VALUES_FOR_ERROR) + term;
            stackPush (&spu->stack, result, VALUES_FOR_ERROR);
            break;}

        case SUB:
            {(spu->ip)++;

            int subtrahend = stackPop (&spu->stack, VALUES_FOR_ERROR);
            int result = stackPop (&spu->stack, VALUES_FOR_ERROR) - subtrahend;
            stackPush (&spu->stack, result, VALUES_FOR_ERROR);
            break;}

        case MUL:
            {(spu->ip)++;

            int result = stackPop (&spu->stack, VALUES_FOR_ERROR) * stackPop (&spu->stack, VALUES_FOR_ERROR);
            stackPush (&spu->stack, result, VALUES_FOR_ERROR);
            break;}

        case DIV:
            {(spu->ip)++;

            int denominator = stackPop (&spu->stack, VALUES_FOR_ERROR);
            int result = stackPop (&spu->stack, VALUES_FOR_ERROR) / denominator;
            stackPush (&spu->stack, result, VALUES_FOR_ERROR);
            break;}

        case OUT:
            {(spu->ip)++;

            int out = stackPop (&spu->stack, VALUES_FOR_ERROR);
            printf ("\n%d\n\n", out);
            break;}

        case HLT:
            {return -1;}
    
        default:
            {return - 2;}
    }

    return 0;
}

int makeCode (SPU *spu, FILE *asmFile)
{
    int cmd = 0;
    fscanf (asmFile, "%d", &cmd);

    if (cmd == 1)
    {
        spu->code[spu->ip] = PUSH;
        (spu->ip)++;

        fscanf (asmFile, "%d", &spu->code[spu->ip]);
        (spu->ip)++;

        return 0;

    } else if (cmd == -1) {
        spu->code[spu->ip] = cmd;
        (spu->ip)++;
        return -1;
    } else {
        spu->code[spu->ip] = cmd;
        (spu->ip)++;
        return 0;
    }
}

CMD assembler (FILE *ptrFile, FILE *asmFile)
{
    char cmd [10] = {};
    fscanf (ptrFile, "%s", cmd);

    if (strcmp(cmd, "PUSH") == 0)
    {
        fprintf (asmFile, "%d ", PUSH);

        int arg = 0;
        fscanf (ptrFile, "%d", &arg);
        fprintf (asmFile, "%d ", arg);

        return PUSH;

    } else if (strcmp(cmd, "POP") == 0)
    {
        fprintf (asmFile, "%d ", POP);
        return POP;

    } else if (strcmp(cmd, "ADD") == 0)
    {
        fprintf (asmFile, "%d ", ADD);
        return ADD;

    } else if (strcmp(cmd, "SUB") == 0)
    {
        fprintf (asmFile, "%d ", SUB);
        return SUB;

    } else if (strcmp(cmd, "MUL") == 0)
    {
        fprintf (asmFile, "%d ", MUL);
        return MUL;

    } else if (strcmp(cmd, "DIV") == 0)
    {
        fprintf (asmFile, "%d ", DIV);
        return DIV;

    } else if (strcmp(cmd, "OUT") == 0)
    {
        fprintf (asmFile, "%d ", OUT);
        return OUT;

    } else if (strcmp(cmd, "HLT") == 0)
    {
        fprintf (asmFile, "%d ", HLT);
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

    FILE * asmFile = fopen (ASM_FILE, "wb");
    if (asmFile == NULL)
    {
        return ERROR;
    }

    while (1)
    {
        int result = assembler(ptrFile, asmFile);

        if (result < -1)
        {
            return 1;
        }

        if (result < 0)
        {
            fclose (asmFile);

            FILE * asmFileRead = fopen (ASM_FILE, "rb");
            if (asmFileRead == NULL)
            {
                return ERROR;
            }

            while (1)
            {
                if (makeCode(spu, asmFile) < 0)
                {
                    spu->ip = 0;
                    return 0;
                }
            }

        }
            
    }
}