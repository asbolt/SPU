#include "sign.h"

int writeSignature (FILE *binFile, FILE *asmFile)
    {
        assert (binFile);
        assert (asmFile);
        
        fprintf (binFile, "%s ", SIGN);
        fprintf (binFile, "%d ", VERSION);

        fseek (asmFile, 0, SEEK_END);
        int size = (int)ftell (asmFile);
        fseek (asmFile, 0, SEEK_SET);

        char *buffer = NULL;
        buffer = (char *)calloc ((size_t)size, sizeof(char));
        if (buffer == NULL)
        {
            return ERROR;
        }

        fread (buffer, sizeof(char), (size_t)size, asmFile);

        int sizeCode = 0;
        for (int i = 0; i < size; i++)
        {
            if (buffer [i] == '\n')
            {
                sizeCode++;
            }

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
            {
                sizeCode++;
            }
        }

        fseek (asmFile, 0, SEEK_SET);

        fprintf (binFile, "%d ", sizeCode);

        return 0;
    }

int checkSignature (FILE *binFile)
    {
        char sign [4] = {};
        fscanf (binFile, "%s", sign);

        int version = 0;
        fscanf (binFile, "%d", &version);
        if (version != VERSION)
        {
            printf ("The file was compiled with not actual processor version\n");
            return ERROR;
        }

        int size = 0;
        fscanf (binFile, "%d", &size);
        return size;
    }