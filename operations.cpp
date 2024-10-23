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

            if (type == 3)
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
                int arg = spu->regs[spu->code[spu->ip]];
                (spu->ip)++;

                stackPush (&spu->stack, arg, VALUES_FOR_ERROR);
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

        case JUMP:
            {(spu->ip)++;

            spu->ip = spu->code[spu->ip];
            break;}

        case JA:
            {(spu->ip)++;
            int a = stackPop(&spu->stack, VALUES_FOR_ERROR);
            if (a < stackPop(&spu->stack, VALUES_FOR_ERROR))
            {
                spu->ip = spu->code[spu->ip];
            } else {
                (spu->ip)++;
            }

            break;}

        case JB:
            {(spu->ip)++;
            int a = stackPop(&spu->stack, VALUES_FOR_ERROR);
            if (a > stackPop(&spu->stack, VALUES_FOR_ERROR))
            {
                spu->ip = spu->code[spu->ip];
            } else {
                (spu->ip)++;
            }

            break;}

        case LABEL:
            break;

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

CMD assembler (FILE *ptrFile, FILE *asmFile, Labels *labels, int *ip)
{
    char cmd [10] = {};
    fscanf (ptrFile, "%s", cmd);

    if (strcmp(cmd, "PUSH") == 0)
    {
        fprintf (asmFile, "%d ", PUSH);
        (*ip)++;

        int arg = 0;
        char str[10] = {};

        if (fscanf (ptrFile, "%d", &arg) == 1)
        {
            fprintf (asmFile, "%d ", 1);
            (*ip)++;
            fprintf (asmFile, "%d ", arg);
            (*ip)++;
            return PUSH;

        } else if (fscanf (ptrFile, "%s", str) == 1)
        {
            if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') == 0)
            {
                fprintf (asmFile, "%d ", 7);
                (*ip)++;

                int arg = 0;

                for (int i = 1; strchr(str, '[') + i < strchr(str, ']'); i++)
                {
                    arg = arg*10 + *(strchr(str, '[') + i) - 48;
                }

                fprintf (asmFile, "%d ", arg);
                (*ip)++;
                return PUSH;

            } else if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, '+') != 0 && strchr(str, 'X') != 0 && strchr(str, 'A') != 0 && strchr(str, 'X') == strchr(str, 'A') + 1)
            {
                fprintf (asmFile, "%d ", 8);
                (*ip)++;

                int arg = 0;
                for (int i = 1; strchr(str, '[') + i < strchr(str, '+'); i++)
                {
                    arg = arg*10 + *(strchr(str, '[') + i) - 48;
                }

                fprintf (asmFile, "%d ", arg);
                (*ip)++;

                fprintf (asmFile, "%d ", 1);
                (*ip)++;
                return PUSH;

            } else if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, '+') != 0 && strchr(str, 'X') != 0 && strchr(str, 'B') != 0 && strchr(str, 'X') == strchr(str, 'B') + 1)
            {
                fprintf (asmFile, "%d ", 8);
                (*ip)++;

                int arg = 0;
                for (int i = 1; strchr(str, '[') + i < strchr(str, '+'); i++)
                {
                    arg = arg*10 + *(strchr(str, '[') + i) - 48;
                }

                fprintf (asmFile, "%d ", arg);
                (*ip)++;

                fprintf (asmFile, "%d ", 2);
                (*ip)++;
                return PUSH;
                
            } else if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, '+') != 0 && strchr(str, 'X') != 0 && strchr(str, 'C') != 0 && strchr(str, 'X') == strchr(str, 'C') + 1)
            {
                fprintf (asmFile, "%d ", 8);
                (*ip)++;

                int arg = 0;
                for (int i = 1; strchr(str, '[') + i < strchr(str, '+'); i++)
                {
                    arg = arg*10 + *(strchr(str, '[') + i) - 48;
                }

                fprintf (asmFile, "%d ", arg);
                (*ip)++;

                fprintf (asmFile, "%d ", 3);
                (*ip)++;
                return PUSH;
                
            } else if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') != 0 && strchr(str, 'A') != 0 && strchr(str, 'X') == strchr(str, 'A') + 1)
            {
                fprintf (asmFile, "%d ", 3);
                (*ip)++;
                fprintf (asmFile, "%d ", 1);
                (*ip)++;
                return PUSH;

            } else if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') != 0 && strchr(str, 'B') != 0 && strchr(str, 'X') == strchr(str, 'B') + 1)
            {
                fprintf (asmFile, "%d ", 3);
                (*ip)++;
                fprintf (asmFile, "%d ", 2);
                (*ip)++;
                return PUSH;
                
            } else if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') != 0 && strchr(str, 'C') != 0 && strchr(str, 'X') == strchr(str, 'C') + 1)
            {
                fprintf (asmFile, "%d ", 3);
                (*ip)++;
                fprintf (asmFile, "%d ", 3);
                (*ip)++;
                return PUSH;
                
            } else if (strchr(str, 'X') != 0 && strchr(str, 'A') != 0 && strchr(str, 'X') == strchr(str, 'A') + 1)
            {
                fprintf (asmFile, "%d ", 2);
                (*ip)++;
                fprintf (asmFile, "%d ", 1);
                (*ip)++;
                return PUSH;

            } else if (strchr(str, 'X') != 0 && strchr(str, 'B') != 0 && strchr(str, 'X') == strchr(str, 'B') + 1)
            {
                fprintf (asmFile, "%d ", 2);
                (*ip)++;
                fprintf (asmFile, "%d ", 2);
                (*ip)++;
                return PUSH;
                
            } else if (strchr(str, 'X') != 0 && strchr(str, 'C') != 0 && strchr(str, 'X') == strchr(str, 'C') + 1)
            {
                fprintf (asmFile, "%d ", 2);
                (*ip)++;
                fprintf (asmFile, "%d ", 3);
                (*ip)++;
                return PUSH;
                
            } else {
                printf ("Syntax error\n");
                return ERROR;
            }
        } else {
                printf ("Syntax error\n");
                return ERROR;
        }
    } else if (strcmp(cmd, "POP") == 0)
    {
        fprintf (asmFile, "%d ", POP);
        (*ip)++;
        return POP;

    } else if (strcmp(cmd, "ADD") == 0)
    {
        fprintf (asmFile, "%d ", ADD);
        (*ip)++;
        return ADD;

    } else if (strcmp(cmd, "SUB") == 0)
    {
        fprintf (asmFile, "%d ", SUB);
        (*ip)++;
        return SUB;

    } else if (strcmp(cmd, "MUL") == 0)
    {
        fprintf (asmFile, "%d ", MUL);
        (*ip)++;
        return MUL;

    } else if (strcmp(cmd, "DIV") == 0)
    {
        fprintf (asmFile, "%d ", DIV);
        (*ip)++;
        return DIV;

    } else if (strcmp(cmd, "OUT") == 0)
    {
        fprintf (asmFile, "%d ", OUT);
        (*ip)++;
        return OUT;

    } else if (strcmp(cmd, "JUMP") == 0)
    {
        fprintf (asmFile, "%d ", JUMP);
        (*ip)++;
        int arg = 0;
        char label [15] = {};

        if (fscanf (ptrFile, "%d", &arg) == 1)
        {
            fprintf (asmFile, "%d ", arg);
            (*ip)++;
            return JUMP;
        } else if (fscanf (ptrFile, "%s", label) == 1)
        {
            for (int i = 0; i < 10; i++)
            {
                if (strcmp(label, labels->labelName[i]) == 0)
                {
                    fprintf (asmFile, "%d ", labels->labelIp[i]);
                    (*ip)++;
                    return JUMP;
                }
            }
                (*ip)++;
        }
        return JUMP;

    } else if (strcmp(cmd, "JA") == 0)
    {
        fprintf (asmFile, "%d ", JA);
        (*ip)++;
        int arg = 0;
        char label [15] = {};

        if (fscanf (ptrFile, "%d", &arg) == 1)
        {
            fprintf (asmFile, "%d ", arg);
            (*ip)++;
            return JA;
        } else if (fscanf (ptrFile, "%s", label) == 1)
        {
            for (int i = 0; i < 10; i++)
            {
                if (strcmp(label, labels->labelName[i]) == 0)
                {
                    fprintf (asmFile, "%d ", labels->labelIp[i]);
                    (*ip)++;
                    return JA;
                }
            }
                (*ip)++;
        }

        return JA;

    } else if (strcmp(cmd, "JB") == 0)
    {
        fprintf (asmFile, "%d ", JB);
        (*ip)++;
        int arg = 0;
        char label [15] = {};

        if (fscanf (ptrFile, "%d", &arg) == 1)
        {
            fprintf (asmFile, "%d ", arg);
            (*ip)++;
            return JB;
        } else if (fscanf (ptrFile, "%s", label) == 1)
        {
            for (int i = 0; i < 10; i++)
            {
                if (strcmp(label, labels->labelName[i]) == 0)
                {
                    fprintf (asmFile, "%d ", labels->labelIp[i]);
                    (*ip)++;
                    return JB;
                }
            }
                (*ip)++;
        }

        return JB;

    } else if (strcmp(cmd, "HLT") == 0)
    {
        fprintf (asmFile, "%d ", HLT);
        (*ip)++;
        return HLT;

    } else {
        if (strchr(cmd, ':') != 0)
        {
            for (int i = 0; i < 10; i++)
            {
                if (strcmp(labels->labelName[i], cmd) == 0)
                {
                    return LABEL;
                }
            }

            for (int i = 0; cmd[i] != '\0'; i++)
            {
                labels->labelName[labels->labelNumber][i] = cmd[i];
            }

            labels->labelIp[labels->labelNumber] = *ip;

            (labels->labelNumber)++;

            return LABEL;
        } else {
            printf ("Syntax error\n");
            return ERROR;
        }
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

    Labels labels = {};
    int ip = 0;

    firstComp (ptrFile, asmFile, &labels, &ip);

    ip = 0;

    fseek (ptrFile, 0, SEEK_SET);
    fclose (asmFile);

    FILE * asmFile1 = fopen (ASM_FILE, "wb");
    if (asmFile == NULL)
    {
        return ERROR;
    }

    sign (asmFile1, ptrFile);

    while (1)
    {
        int result = assembler(ptrFile, asmFile1, &labels, &ip);

        if (result < -1)
        {
            return 1;
        }

        if (result < 0)
        {
            fclose (asmFile1);

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
        if (buffer [i] == '\n')
            sizeCode++;

        if ((buffer [i] == 'P' && buffer [i + 1] == 'U' && buffer [i + 2] == 'S' && buffer [i + 3] == 'H'))
        {
            sizeCode += 2;
        }

        if ((buffer [i] == 'P' && buffer [i + 1] == 'O' && buffer [i + 2] == 'P'))
        {
            sizeCode += 2;
        }

        if ((buffer [i] == 'J' && buffer [i + 1] == 'U' && buffer [i + 2] == 'M' && buffer [i + 3] == 'P'))
        {
            sizeCode++;
        }

        if ((buffer [i] == 'J' && buffer [i + 1] == 'A'))
        {
            sizeCode++;
        }

        if ((buffer [i] == 'J' && buffer [i + 1] == 'B'))
        {
            sizeCode ++;
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

int firstComp (FILE *ptrFile, FILE *asmFile, Labels *labels, int *ip)
{
    while (1)
    {
        int result = assembler(ptrFile, asmFile, labels, ip);

        if (result < -1)
        {
            return 1;
        }

        if (result < 0)
        {
            return 0;
        }      
    }
}