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

        *ip = 0;

        while (true)
        {
            char *command = (char *)calloc (MAX_COMMAND_SIZE, sizeof(char));
            if (command == NULL)
            {
                return ERROR_;
            }

            fgets (command, MAX_COMMAND_SIZE, asmFile);

            int result = readCommand(command, ip, binFile, labels);

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

REGS determineRegister (char *command)
{
    if (strchr(command, 'X') != 0)
    {
        if (strchr(command, 'A') != 0)
        {
            return AX;
        } else if (strchr(command, 'B') != 0)
        {
            return BX;
        } else if (strchr(command, 'C') != 0)
        {
            return CX;
        } else {
            return REG_ERROR;
        }
    } else {
        return REG_ERROR;
    }
}

int readImmediate (char *command)
{
    const char numbers [] = "1234567890";
    char *address = strpbrk (command, numbers);
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

int checkRAM (char *command)
{
    if (strchr(command, '[') != 0 && strchr(command, ']') != 0)
    {
        if (strchr(command, '+') != 0)
        {
            return RAM_WITH_PLUS;
        } else {
            return RAM_NUMBER;
        }
    }

    return 0;
}

int determineLabel (char *command, Labels *labels, FILE *binFile, int *ip)
{
    for (int label = 0; label < numberLabels; label++)
    {
        if (strstr(command, labels->labelName[label]) != 0)
        {
            fprintf (binFile, "%d ", labels->labelIp[label]);
            (*ip)++;
            return 1;
        }
    }

    return 0;
}

CMD readCommand (char *command, int *ip, FILE *binFile, Labels *labels)
{
    assert (binFile);
    assert (labels);
    assert (ip);
    assert (command);

    #define processInsn(name) else if(strstr(command, #name) != 0) { \
           fprintf (binFile, "%d ", name);                           \
           (*ip)++;                                                  \
           return name;                                              \
       }

    #define jump(name) else if (strstr(command, #name) != 0)   \
    {                                                                \
        fprintf (binFile, "%d ", name);                              \
        (*ip)++;                                                     \
                                                                     \
        if (determineLabel (command, labels, binFile, ip) == 0)      \
        {                                                            \
            fprintf (binFile, "%d ", readImmediate(command));        \
            (*ip)++;                                                 \
        }                                                            \
                                                                     \
        return name;                                                 \
    }                                                                \

    if (strstr(command, "PUSH") != 0)
    {
        fprintf (binFile, "%d ", PUSH);
        (*ip)++;

        int pushMode = checkRAM(command) + determineRegister(command);
        fprintf(binFile, "%d ", pushMode);
        (*ip)++;

        if (pushMode == ARG || pushMode == RAM_ARG || pushMode == RAM_ARG_AND_REG)
        {
            fprintf(binFile, "%d ", readImmediate(command));
            (*ip)++;
        }

        if (pushMode == REG || pushMode == RAM_REG || pushMode == RAM_ARG_AND_REG)
        {
            fprintf(binFile, "%d ", determineRegister(command));
            (*ip)++;
        }

        return PUSH;

    }
    processInsn(POP)
    processInsn(ADD)
    processInsn(SUB)
    processInsn(MUL)
    processInsn(DIV)
    processInsn(OUT)
    processInsn(HLT)
    jump (JUMP)
    jump (JA)
    jump (JB)
      else {
        return ERROR;
    }
}
