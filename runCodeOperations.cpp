#include "stack.h"
#include "runCodeOperations.h"
#include "stackUserInterface.h"
#include <cstring>

int runCodeCommand (SPU *spu)
    {
        assert (spu);

        int cmd = spu->code[spu->ip];

        switch (cmd)
        {
            case PUSH:
                {
                    (spu->ip)++;
                    int type = spu->code[spu->ip];
                    (spu->ip)++;

                    if (type == RAM_REG)
                    {
                        int address = spu->regs[spu->code[spu->ip]];
                        (spu->ip)++;

                        stackPush (&spu->stack, spu->RAM[address], VALUES_FOR_ERROR);
                        break;
                    }

                    if (type == RAM_ARG_AND_REG)
                    {
                        int address = spu->code[spu->ip];
                        (spu->ip)++;
                        address += spu->regs[spu->code[spu->ip]];
                        (spu->ip)++;

                        stackPush (&spu->stack, spu->RAM[address], VALUES_FOR_ERROR);
                        break;
                    }

                    if (type == RAM_ARG)
                    {
                        int address = 0;

                        int arg = spu->code[spu->ip];
                        (spu->ip)++;
                        address += arg;

                        stackPush (&spu->stack, spu->RAM[address], VALUES_FOR_ERROR);
                        break;
                    }

                    if (type == ARG)
                    {
                        int arg = spu->code[spu->ip];
                        (spu->ip)++;
                        stackPush (&spu->stack, arg, VALUES_FOR_ERROR);
                        break;
                    }

                    if (type == REG)
                    {
                        int arg = spu->regs[spu->code[spu->ip]];
                        (spu->ip)++;

                        stackPush (&spu->stack, arg, VALUES_FOR_ERROR);
                        break;
                    }

                    break;
                }

            case POP:
                {
                    (spu->ip)++;

                    stackPop (&spu->stack, VALUES_FOR_ERROR);
                    break;
                }

            case ADD:
                {
                    (spu->ip)++;

                    int term = stackPop (&spu->stack, VALUES_FOR_ERROR);
                    int result = stackPop (&spu->stack, VALUES_FOR_ERROR) + term;
                    stackPush (&spu->stack, result, VALUES_FOR_ERROR);
                    break;
                }

            case SUB:
                {
                    (spu->ip)++;

                    int subtrahend = stackPop (&spu->stack, VALUES_FOR_ERROR);
                    int result = stackPop (&spu->stack, VALUES_FOR_ERROR) - subtrahend;
                    stackPush (&spu->stack, result, VALUES_FOR_ERROR);
                    break;
                }

            case MUL:
                {
                    (spu->ip)++;

                    int result = stackPop (&spu->stack, VALUES_FOR_ERROR) * stackPop (&spu->stack, VALUES_FOR_ERROR);
                    stackPush (&spu->stack, result, VALUES_FOR_ERROR);
                    break;
                }

            case DIV:
                {
                    (spu->ip)++;

                    int denominator = stackPop (&spu->stack, VALUES_FOR_ERROR);
                    int result = stackPop (&spu->stack, VALUES_FOR_ERROR) / denominator;
                    stackPush (&spu->stack, result, VALUES_FOR_ERROR);
                    break;
                }

            case OUT:
                {
                    (spu->ip)++;

                    int out = stackPop (&spu->stack, VALUES_FOR_ERROR);
                    printf ("\n%d\n\n", out);
                    break;
                }

            case JUMP:
                {
                    (spu->ip)++;

                    spu->ip = spu->code[spu->ip];
                    break;
                }

            case JA:
                {
                    (spu->ip)++;
                    int a = stackPop(&spu->stack, VALUES_FOR_ERROR);
                    if (a < stackPop(&spu->stack, VALUES_FOR_ERROR))
                    {
                        spu->ip = spu->code[spu->ip];

                    } else {

                        (spu->ip)++;
                    }

                    break;
                }

            case JB:
                {
                    (spu->ip)++;
                    int a = stackPop(&spu->stack, VALUES_FOR_ERROR);
                    if (a > stackPop(&spu->stack, VALUES_FOR_ERROR))
                    {
                        spu->ip = spu->code[spu->ip];

                    } else {

                        (spu->ip)++;
                    }

                    break;
                }

            case LABEL:

                    break;

            case HLT:

                    return HLT;

            default:

                    return ERROR;
        }

        return 0;
    }

int readCode (SPU *spu, FILE *binFile)
    {
        assert (spu);
        assert (binFile);

        int cmd = 0;
        fscanf (binFile, "%d", &cmd);

        if (cmd == HLT) 
        {
            spu->code[spu->ip] = cmd;
            (spu->ip)++;
            return HLT;

        } else {

            spu->code[spu->ip] = cmd;
            (spu->ip)++;
            return 0;
        }

       return -1;
    }

int makeCode (SPU *spu)
    {
            FILE * binFileRead = fopen (BINARY_FILE, "rb");
            if (binFileRead == NULL)
            {
                return ERROR_;
            }

            spuCtor (spu, checkSignature (binFileRead));

            while (true)
            {
                if (readCode(spu, binFileRead) < CORRECT_)
                {
                    spu->ip = 0;
                    return CORRECT_;
                }
            }

           fclose (binFileRead);

            return CORRECT_;
    }

int runCode (SPU *spu)
{
    while (true)
    {
        if (runCodeCommand (spu) < CORRECT_)
        {
            spuDtor (spu);
            return 0;
        }
    }

    return 1;
}