#include "spu.h"
#include "assembler.h"


int main ()
{ 
    SPU spu = {};

    if (readFile (&spu) == ERROR_)
    {
        return 1;
    }

    if (makeCode(&spu) == ERROR_)
    {
        return 1;
    }

    if (runCode (&spu) == 1)
    {
        return 1;
    }

    return 0;
}