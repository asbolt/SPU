#include "assembler.h"

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

        // TODO use your Onegin as library to read assembly code
        seekLabels (asmFile, binFile, &labels, &ip);

        ip = 0;

        fseek (asmFile, 0, SEEK_SET);

        fseek (binFile, 0, SEEK_SET);
        writeSignature (binFile, asmFile);        

        while (true)
        {
            int result = makeBinFile(asmFile, binFile, &labels, &ip);
            if (result < HLT)
            {
                return ERROR_;
            }

            if (result < CORRECT_)
            {
                fclose (binFile);

                return CORRECT_;
            }      
        }
    }

CMD makeBinFile (FILE *asmFile, FILE *binFile, Labels *labels, int *ip)
    {
        assert (asmFile);
        assert (binFile);
        assert (labels);
        assert (ip);

        // TODO separate functions for: determining register, reading immediate (const), reading command, checking for '[', ']'

        char cmd [MAX_COMMAND_SIZE] = {};
        fscanf (asmFile, "%s", cmd);

        if (strcmp(cmd, "PUSH") == 0)
        {
            fprintf (binFile, "%d ", PUSH);
            (*ip)++;

            int arg = 0;
            char str[MAX_COMMAND_SIZE] = {};

            if (fscanf (asmFile, "%d", &arg) == 1)
            {
                fprintf (binFile, "%d ", ARG);
                (*ip)++;
                fprintf (binFile, "%d ", arg);
                (*ip)++;
                return PUSH;

            } else if (fscanf (asmFile, "%s", str) == 1)
            {
                if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') == 0)
                {
                    fprintf (binFile, "%d ", RAM_ARG);
                    (*ip)++;

                    int argRam = 0;

                    for (int i = 1; strchr(str, '[') + i < strchr(str, ']'); i++)
                    {
                        argRam = argRam*basisNumberSystem + *(strchr(str, '[') + i) - '0';
                    }

                    fprintf (binFile, "%d ", arg);
                    (*ip)++;
                    return PUSH;

                } else if (pushRamArgAndReg (binFile, str, ip, 'A', AX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamArgAndReg (binFile, str, ip, 'B', BX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamArgAndReg (binFile, str, ip, 'C', CX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamReg (binFile, str, ip, 'A', AX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamReg (binFile, str, ip, 'B', BX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamReg (binFile, str, ip, 'C', CX) == PUSH)
                {
                    return PUSH;

                } else if (pushReg (binFile, str, ip, 'A', AX) == PUSH)
                {
                    return PUSH;

                } else if (pushReg (binFile, str, ip, 'B', BX) == PUSH)
                {
                    return PUSH;

                } else if (pushReg (binFile, str, ip, 'C', CX) == PUSH)
                {
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
            fprintf (binFile, "%d ", POP);
            (*ip)++;
            return POP;

        } else if (strcmp(cmd, "ADD") == 0)
        {
            fprintf (binFile, "%d ", ADD);
            (*ip)++;
            return ADD;

        } else if (strcmp(cmd, "SUB") == 0)
        {
            fprintf (binFile, "%d ", SUB);
            (*ip)++;
            return SUB;

        } else if (strcmp(cmd, "MUL") == 0)
        {
            fprintf (binFile, "%d ", MUL);
            (*ip)++;
            return MUL;

        } else if (strcmp(cmd, "DIV") == 0)
        {
            fprintf (binFile, "%d ", DIV);
            (*ip)++;
            return DIV;

        } else if (strcmp(cmd, "OUT") == 0)
        {
            fprintf (binFile, "%d ", OUT);
            (*ip)++;
            return OUT;

        } else if (jumps (asmFile, binFile, cmd, ip, labels, "JUMP", JUMP) == JUMP)
        {
            return JUMP;

        } else if (jumps (asmFile, binFile, cmd, ip, labels, "JA", JA) == JA)
        {
            return JA;

        } else if (jumps (asmFile, binFile, cmd, ip, labels, "JB", JB) == JB)
        {
            return JB;

        } else if (strcmp(cmd, "HLT") == 0)
        {
            fprintf (binFile, "%d ", HLT);
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

int seekLabels (FILE *asmFile, FILE *binFile, Labels *labels, int *ip)
    {
        assert (asmFile);
        assert (binFile);
        assert (labels);
        assert (ip);

        while (true)
        {
            int result = makeBinFile(asmFile, binFile, labels, ip);

            if (result < HLT)
            {
                return ERROR_;
            }

            if (result < CORRECT_)
            {
                return CORRECT_;
            }      
        }
    }

int pushRamArgAndReg (FILE *binFile, char *str, int *ip, char regLetter, int regNumber)
    {
        assert (binFile);
        assert (str);
        assert (ip);

        if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, '+') != 0 
            && strchr(str, 'X') != 0 && strchr(str, regLetter) != 0 && strchr(str, 'X') == strchr(str, regLetter) + 1)
            {
                fprintf (binFile, "%d ", RAM_ARG_AND_REG);
                (*ip)++;

                int arg = 0;
                for (int i = 1; strchr(str, '[') + i < strchr(str, '+'); i++)
                {
                    arg = arg*10 + *(strchr(str, '[') + i) - '0';
                }

                fprintf (binFile, "%d ", arg);
                (*ip)++;
                fprintf (binFile, "%d ", regNumber);
                (*ip)++;

                return PUSH;
            }

        return ERROR;
    }

int pushRamReg (FILE *binFile, char *str, int *ip, char regLetter, int regNumber)
    {
        assert (binFile);
        assert (str);
        assert (ip);

        if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') != 0 && strchr(str, regLetter) != 0 
            && strchr(str, 'X') == strchr(str, regLetter) + 1)
            {
                fprintf (binFile, "%d ", RAM_REG);
                (*ip)++;
                fprintf (binFile, "%d ", regNumber);
                (*ip)++;

                return PUSH;
            }

        return ERROR;
    }

int pushReg (FILE *binFile, char *str, int *ip, char regLetter, int regNumber)
    {
        if (strchr(str, 'X') != 0 && strchr(str, regLetter) != 0 && strchr(str, 'X') == strchr(str, regLetter) + 1)
        {
            fprintf (binFile, "%d ", REG);
            (*ip)++;
            fprintf (binFile, "%d ", regNumber);
            (*ip)++;

            return PUSH;
        }

        return ERROR;
    }

int jumps (FILE *asmFile, FILE *binFile, char *cmd, int *ip, Labels *labels, const char *jumpType, int jumpNumber)
    {
        if (strcmp(cmd, jumpType) == 0)
        {
            fprintf (binFile, "%d ", jumpNumber);
            (*ip)++;
            int arg = 0;
            char label [15] = {};

            if (fscanf (asmFile, "%d", &arg) == 1)
            {
                fprintf (binFile, "%d ", arg);
                (*ip)++;
                return JUMP;

            } else if (fscanf (asmFile, "%s", label) == 1)
            {
                for (int i = 0; i < MAX_COMMAND_SIZE; i++)
                {
                    if (strcmp(label, labels->labelName[i]) == 0)
                    {
                        fprintf (binFile, "%d ", labels->labelIp[i]);
                        (*ip)++;
                        return jumpNumber;
                    }
                }
                    (*ip)++;
            }

            return jumpNumber;
        }

        return ERROR;
    }