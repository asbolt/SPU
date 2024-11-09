#include "assembler.h"
#include "sign.h"

ERROR_CODES readFile (SPU *spu)
    {
        assert (spu);

        FILE * asmFile = fopen (ASM_FILE, "rb");
        if (asmFile == NULL)
        {
            return ERROR_;
        }

        FILE * binFile = fopen (BINARY_FILE, "wb");
        if (binFile == NULL)
        {
            return ERROR_;
        }

        Labels labels = {};
        int ip = 0;

        compilation (asmFile, binFile, &labels, &ip);

        ip = 0;
        fseek (asmFile, 0, SEEK_SET);
        fseek (binFile, 0, SEEK_SET);

        writeSignature (binFile, asmFile);        

        compilation (asmFile, binFile, &labels, &ip);

        return CORRECT_;
    }

int compilation (FILE *asmFile, FILE *binFile, Labels *labels, int *ip)
    {
        assert (asmFile);
        assert (binFile);
        assert (labels);
        assert (ip);

        while (true)
        {
            char *str = (char *)calloc (MAX_COMMAND_SIZE, sizeof(char));
            if (str == NULL)
            {
                return ERROR_;
            }

            fgets (str, MAX_COMMAND_SIZE, asmFile);

            int result = readingCommand(str, ip, binFile, labels);

            if (result < HLT)
            {
                return ERROR_;
            }

            if (result == HLT)
            {
                return CORRECT_;
            }      
        }
    }

REGS determiningRegister (char *str)
{
    if (strchr(str, 'X') != 0)
    {
        if (strchr(str, 'A') != 0)
        {
            return AX;
        } else if (strchr(str, 'B') != 0)
        {
            return BX;
        } else if (strchr(str, 'C') != 0)
        {
            return CX;
        } else {
            return REG_ERROR;
        }
    } else {
        return REG_ERROR;
    }
}

int readingImmediate (char *str)
{
    const char numbers [] = "1234567890";
    char *address = strpbrk (str, numbers);
    int arg = 0;
    int step = 0;

    while (true)
    {
        address = strpbrk (address + step, numbers);
        if (address == NULL)
        {
            return arg;
        }
        
        arg = arg*10 + *address - '0'; 

        step++;
    }

    return arg;
}

int checkRAM (char *str)
{
    if (strchr(str, '[') != 0 && strchr(str, ']') != 0)
    {
        if (strchr(str, '+') != 0)
        {
            return 8;
        } else {
            return 5;
        }
    }

    return 0;
}

int determiningLabel (char *str, Labels *labels, FILE *binFile, int *ip)
{
    for (int i = 0; i < MAX_COMMAND_SIZE; i++)
    {
        if (strstr(str, labels->labelName[i]) != 0)
        {
            fprintf (binFile, "%d ", labels->labelIp[i]);
            (*ip)++;
            return 1;
        }
    }

    return 0;
}

CMD readingCommand (char *str, int *ip, FILE *binFile, Labels *labels)
{
    assert (binFile);
    assert (labels);
    assert (ip);
    assert (str);

    if (strstr(str, "PUSH") != 0)
    {
        fprintf (binFile, "%d ", PUSH);
        (*ip)++;

        int pushMode = checkRAM(str) + determiningRegister(str);
        fprintf(binFile, "%d ", pushMode);
        (*ip)++;

        if (pushMode == ARG || pushMode == RAM_ARG || pushMode == RAM_ARG_AND_REG)
        {
            fprintf(binFile, "%d ", readingImmediate(str));
            (*ip)++;
        }

        if (pushMode == REG || pushMode == RAM_REG || pushMode == RAM_ARG_AND_REG)
        {
            fprintf(binFile, "%d ", determiningRegister(str));
            (*ip)++;
        }

        return PUSH;

    } else if (strstr(str, "POP") != 0)
    {
        fprintf (binFile, "%d ", PUSH);
        (*ip)++;
        return POP;
    } else if (strstr(str, "ADD") != 0)
    {
        fprintf (binFile, "%d ", ADD);
        (*ip)++;
        return ADD;
    } else if (strstr(str, "SUB") != 0)
    {
        fprintf (binFile, "%d ", SUB);
        (*ip)++;
        return SUB;
    } else if (strstr(str, "MUL") != 0)
    {
        fprintf (binFile, "%d ", MUL);
        (*ip)++;
        return MUL;
    } else if (strstr(str, "DIV") != 0)
    {
        fprintf (binFile, "%d ", DIV);
        (*ip)++;
        return DIV;
    } else if (strstr(str, "OUT") != 0)
    {
        fprintf (binFile, "%d ", OUT);
        (*ip)++;
        return OUT;
    } else if (strstr(str, "HLT") != 0)
    {
        fprintf (binFile, "%d ", HLT);
        (*ip)++;
        return HLT;
    } else if (strstr(str, "JUMP") != 0)
    {
        fprintf (binFile, "%d ", JUMP);
        (*ip)++;
        
        if (determiningLabel (str, labels, binFile, ip) == 0)
        {
            fprintf (binFile, "%d ", readingImmediate(str));
            (*ip)++;
        }

        return JUMP;

    } else if (strstr(str, "JA") != 0)
    {
        fprintf (binFile, "%d ", JA);
        (*ip)++;
        
        if (determiningLabel (str, labels, binFile, ip) == 0)
        {
            fprintf (binFile, "%d ", readingImmediate(str));
            (*ip)++;
        }

        return JA;

    } else if (strstr(str, "JB") != 0)
    {
        fprintf (binFile, "%d ", JUMP);
        (*ip)++;
        
        if (determiningLabel (str, labels, binFile, ip) == 0)
        {
            fprintf (binFile, "%d ", readingImmediate(str));
            (*ip)++;
        }

        return JB;
    } else {
        return ERROR;
    }
}
