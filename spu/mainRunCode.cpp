#include "spu.h"
#include "runCodeOperations.h"

int main ()
{ 
    SPU spu = {};

    if (readCode(&spu) == ERROR_)
    {
        return 1;
    }

    if (runCode (&spu) == 1)
    {
        return 1;
    }

    return 0;
}