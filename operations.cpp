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
            {
            (spu->ip)++;
            int type = spu->code[spu->ip];
            (spu->ip)++;

            if (type == 9)
            {
                int address = spu->regs[spu->code[spu->ip]];
                (spu->ip)++;

                stackPush (&spu->stack, spu->RAM[address], VALUES_FOR_ERROR);
                return PUSH;
            }

            if (type == 8)
            {
                int address = spu->code[spu->ip];
                (spu->ip)++;
                address += spu->regs[spu->code[spu->ip]];
                (spu->ip)++;

                stackPush (&spu->stack, spu->RAM[address], VALUES_FOR_ERROR);
                return PUSH;
            }

            if (type == 7)
            {
                int address = 0;

                int arg = spu->code[spu->ip];
                (spu->ip)++;
                address += arg;

                stackPush (&spu->stack, spu->RAM[address], VALUES_FOR_ERROR);
                return PUSH;
            }

            if (type == 1)
            {
                int arg = spu->code[spu->ip];
                (spu->ip)++;
                stackPush (&spu->stack, arg, VALUES_FOR_ERROR);
            }

            if (type == 2)
            {
                int arg = spu->regs[spu->ip];
                (spu->ip)++;
                stackPush (&spu->stack, spu->regs[arg], VALUES_FOR_ERROR);
            }

            break;
            }

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
        char str[2] = {};
        char reg[2] = {};

        if (fscanf (ptrFile, "%d", &arg) == 1)
        {
            fprintf (asmFile, "%d ", 1);
            fprintf (asmFile, "%d ", arg);
            
        } else if (fscanf (ptrFile, "%s", str) == 1)
        {
            if (str[0] == '[')
            {

                if (fscanf (ptrFile, "%d", &arg) == 1)
                {
                    if (fscanf (ptrFile, "%s", str) == 1)
                    {
                        if (str[0] == '+')
                        {
                            fprintf (asmFile, "%d ", 8);
                            fprintf (asmFile, "%d ", arg);
                            fscanf (ptrFile, "%s", reg);

                            if (strcmp(reg, "AX") == 0)
                            {
                                fprintf (asmFile, "%d ", 1);
                            } else if (strcmp(reg, "BX") == 0)
                            {
                                fprintf (asmFile, "%d ", 2);
                            } else if (strcmp(reg, "CX") == 0)
                            {
                                fprintf (asmFile, "%d ", 3);
                            } else {
                                printf ("Syntax error\n");
                                return ERROR;
                            }

                            fscanf (ptrFile, "%s", str);
                            if (str[0] == ']')
                            {
                                return PUSH;
                            } else {
                                printf ("Syntax error\n");
                                return ERROR;
                            }

                        } else if (str[0] == ']')
                        {
                            fprintf (asmFile, "%d ", 7);
                            fprintf (asmFile, "%d ", arg);
                            return PUSH;
                        } else 
                        {
                            printf ("Syntax error\n");
                            return ERROR;
                        }
                    }
                } else if (fscanf (ptrFile, "%s", reg) == 1)
                {
                    if (reg[1] == 'X')
                    {
                        if (strcmp(reg, "AX") == 0)
                        {
                            fprintf (asmFile, "%d ", 9);
                            fprintf (asmFile, "%d ", 1);
                        } else if (strcmp(reg, "BX") == 0)
                        {
                            fprintf (asmFile, "%d ", 9);
                            fprintf (asmFile, "%d ", 2);
                        } else if (strcmp(reg, "CX") == 0)
                        {
                            fprintf (asmFile, "%d ", 9);
                            fprintf (asmFile, "%d ", 3);
                        } else {
                            printf ("Syntax error\n");
                            return ERROR;
                        }
                    } else {
                        printf ("Syntax error\n");
                        return ERROR;
                    }

                    fscanf (ptrFile, "%s", str);
                            if (str[0] == ']')
                            {
                                return PUSH;
                            } else {
                                printf ("Syntax error\n");
                                return ERROR;
                            }
                }
            } else if (str[1] == 'X')
            {
                fprintf (asmFile, "%d ", 2);

                if (strcmp(str, "AX") == 0)
                {
                    fprintf (asmFile, "%d ", 1);
                } else if (strcmp(str, "BX") == 0)
                {
                    fprintf (asmFile, "%d ", 2);
                } else if (strcmp(str, "CX") == 0)
                {
                    fprintf (asmFile, "%d ", 3);
                } else {
                    printf ("Syntax error\n");
                    return ERROR;
                }
            }
        }

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

    sign (asmFile, ptrFile);

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

            codeCtor (checkSign (asmFileRead), spu);

            while (1)
            {
                if (makeCode(spu, asmFileRead) < 0)
                {
                    spu->ip = 0;
                    return 0;
                }
            }
        }      
    }
}

int sign (FILE *asmFile, FILE *ptrFile)
{
    fprintf (asmFile, "%s ", SIGN);
    fprintf (asmFile, "%d ", VERSION);

    fseek (ptrFile, 0, SEEK_END);
    int size = ftell (ptrFile);
    fseek (ptrFile, 0, SEEK_SET);

    char *buffer = NULL;
    buffer = (char *)calloc (size, sizeof(char));
    if (buffer == NULL)
    {
        return ERROR;
    }

    fread (buffer, sizeof(char), size, ptrFile);

    int sizeCode = 0;
    for (int i = 0; i < size; i++)
    {
        if (buffer [i] == ' ')
            sizeCode++;

        if ((buffer [i] == 'P' && buffer [i + 1] == 'U' && buffer [i + 2] == 'S' && buffer [i + 3] == 'H'))
        {
            sizeCode += 2;
        }

        if (buffer [i] == '+')
            sizeCode++;
    }

    fseek (ptrFile, 0, SEEK_SET);
    
    fprintf (asmFile, "%d ", sizeCode);

    return 0;
}

int checkSign (FILE *asmFile)
{
    char sign [4] = {};
    fscanf (asmFile, "%s", sign);

    int version = 0;
    fscanf (asmFile, "%d", &version);
    if (version != VERSION)
    {
        printf ("The file was compiled with not actual processor version\n");
        return ERROR;
    }

    int size = 0;
    fscanf (asmFile, "%d", &size);
    return size;
}

int codeCtor (int size, SPU *spu)
{
    spu->code = (int*)calloc(size, sizeof(int));
    if (spu->code == NULL)
    {
        return ERROR;
    }

    return 0; 
}