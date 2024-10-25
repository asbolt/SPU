#include "assembler.h"

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
                fprintf (asmFile, "%d ", ARG);
                (*ip)++;
                fprintf (asmFile, "%d ", arg);
                (*ip)++;
                return PUSH;

            } else if (fscanf (ptrFile, "%s", str) == 1)
            {
                if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') == 0)
                {
                    fprintf (asmFile, "%d ", RAM_ARG);
                    (*ip)++;

                    int argRam = 0;

                    for (int i = 1; strchr(str, '[') + i < strchr(str, ']'); i++)
                    {
                        argRam = argRam*10 + *(strchr(str, '[') + i) - 48;
                    }

                    fprintf (asmFile, "%d ", arg);
                    (*ip)++;
                    return PUSH;

                } else if (pushRamArgAndReg (asmFile, str, ip, 'A', AX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamArgAndReg (asmFile, str, ip, 'B', BX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamArgAndReg (asmFile, str, ip, 'C', CX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamReg (asmFile, str, ip, 'A', AX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamReg (asmFile, str, ip, 'B', BX) == PUSH)
                {
                    return PUSH;

                } else if (pushRamReg (asmFile, str, ip, 'C', CX) == PUSH)
                {
                    return PUSH;

                } else if (pushReg (asmFile, str, ip, 'A', AX) == PUSH)
                {
                    return PUSH;

                } else if (pushReg (asmFile, str, ip, 'B', BX) == PUSH)
                {
                    return PUSH;

                } else if (pushReg (asmFile, str, ip, 'C', CX) == PUSH)
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

        } else if (jumps (ptrFile, asmFile, cmd, ip, labels, "JUMP", JUMP) == JUMP)
        {
            return JUMP;

        } else if (jumps (ptrFile, asmFile, cmd, ip, labels, "JA", JA) == JA)
        {
            return JA;

        } else if (jumps (ptrFile, asmFile, cmd, ip, labels, "JB", JB) == JB)
        {
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

int pushRamArgAndReg (FILE *asmFile, char *str, int *ip, char regLetter, int regNumber)
    {
        if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, '+') != 0 
            && strchr(str, 'X') != 0 && strchr(str, regLetter) != 0 && strchr(str, 'X') == strchr(str, regLetter) + 1)
            {
                fprintf (asmFile, "%d ", RAM_ARG_AND_REG);
                (*ip)++;

                int arg = 0;
                for (int i = 1; strchr(str, '[') + i < strchr(str, '+'); i++)
                {
                    arg = arg*10 + *(strchr(str, '[') + i) - 48;
                }

                fprintf (asmFile, "%d ", arg);
                (*ip)++;
                fprintf (asmFile, "%d ", regNumber);
                (*ip)++;

                return PUSH;
            }

        return ERROR;
    }

int pushRamReg (FILE *asmFile, char *str, int *ip, char regLetter, int regNumber)
    {
        if (strchr(str, '[') != 0 && strchr(str, ']') != 0 && strchr(str, 'X') != 0 && strchr(str, regLetter) != 0 
            && strchr(str, 'X') == strchr(str, regLetter) + 1)
            {
                fprintf (asmFile, "%d ", RAM_REG);
                (*ip)++;
                fprintf (asmFile, "%d ", regNumber);
                (*ip)++;

                return PUSH;
            }

        return ERROR;
    }

int pushReg (FILE *asmFile, char *str, int *ip, char regLetter, int regNumber)
    {
        if (strchr(str, 'X') != 0 && strchr(str, regLetter) != 0 && strchr(str, 'X') == strchr(str, regLetter) + 1)
        {
            fprintf (asmFile, "%d ", REG);
            (*ip)++;
            fprintf (asmFile, "%d ", regNumber);
            (*ip)++;

            return PUSH;
        }

        return ERROR;
    }

int jumps (FILE *ptrFile, FILE *asmFile, char *cmd, int *ip, Labels *labels, const char *jumpType, int jumpNumber)
    {
        if (strcmp(cmd, jumpType) == 0)
        {
            fprintf (asmFile, "%d ", jumpNumber);
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
                        return jumpNumber;
                    }
                }
                    (*ip)++;
            }

            return jumpNumber;
        }

        return ERROR;
    }