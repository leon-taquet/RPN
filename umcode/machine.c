#include "machine.h"

#include "parse.h"
#include "mainmem.h"
#include "cpu.h"

void execute_program(FILE* fp)
{
    MainMem mem = MainMem_new(parse_instructions(fp));
    
    Cpu cpu = Cpu_new(mem);

    fetch_decode_execute(cpu);

    Cpu_free(&cpu);
}
