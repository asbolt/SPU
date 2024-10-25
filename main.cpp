#include "spu.h"
#include "assembler.h"


int main ()
{ 
    SPU spu = {};
    stackCtor (&spu.stack, 10);

    if (readFile (&spu) == 1)
        return 1;

    spu.regs[1] = 7;
    spu.RAM[78] = 1;
    spu.RAM[9] = 3;

    while (1)
    {
        if (run (&spu) < 0)
        {
            stackDtor (&spu.stack, VALUES_FOR_ERROR);
            codeDtor(&spu);
            return 0;
        }
    }


    return 0;
}