#include "spu.h"

// TODO переделать структуру лэйблс
// TODO makefile
// TODO переименовать файлы
// TODO вынести файлы в папки

ERROR_CODES labelCtor (Labels *labels)
    {
        labels->labelName = (char**)calloc ((size_t)numberLabels, sizeof(char));
        if (labels->labelName == NULL)
        {
            return ERROR_;
        }

        for (int i = 0; i < numberLabels; i++)
        {
            labels->labelName[i] = (char*)calloc ((size_t)SIZE_LABEL, sizeof(char));
            if (labels->labelName[i] == NULL)
            {
                return ERROR_;
            }
        }

        labels->labelIp = (int*)calloc ((size_t)numberLabels, sizeof(int));
        if (labels->labelIp == NULL)
        {
            return ERROR_;
        }

        labels->labelNumber = 0;

        return CORRECT_;
    }

ERROR_CODES labelDtor (Labels *labels)
    {
        assert (labels);

        free (labels->labelName);
        free (labels->labelIp);

        return CORRECT_;
    }

ERROR_CODES codeArrayCtor (int size, SPU *spu)
    {
        spu->code = (int*)calloc((size_t)size, sizeof(int));
        if (spu->code == NULL)
        {
            return ERROR_;
        }

        return CORRECT_; 
    }

ERROR_CODES codeArrayDtor (int *code)
    {
        assert (code);

        free (code);

        return CORRECT_;
    }

ERROR_CODES spuCtor (SPU *spu, int size)
    {
        assert (spu);

        if (codeArrayCtor (size, spu) == ERROR_)
        {
           return ERROR_;
        }

        if (stackCtor (&spu->stack, DEFAULT_STACK_SIZE) != STACK_GOOD)
        {
            return ERROR_;
        }

        if (labelCtor (&spu->labels) == ERROR_)
        {
            return ERROR_;
        }

        spu->regs = (int*)calloc((size_t)DEFAULT_REG_SIZE, sizeof(int));
        if (spu->regs == NULL)
        {
            return ERROR_;
        }

        spu->RAM = (int*)calloc((size_t)DEFAULT_RAM_SIZE, sizeof(int));
        if (spu->RAM == NULL)
        {
            return ERROR_;
        }
        
        spu->ip = 0;

        return CORRECT_;
    }

ERROR_CODES spuDtor (SPU *spu)
    {
        assert (spu);
    
        codeArrayDtor (spu->code);
        labelDtor (&spu->labels);
        stackDtor (&spu->stack, VALUES_FOR_ERROR);
    
        free (spu->regs);
        free (spu->RAM);
    
        return CORRECT_;
    }