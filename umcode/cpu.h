#ifndef CPU_INCLUDED
#define CPU_INCLUDED

#include "mainmem.h"

#include <stdbool.h>

#define T Cpu

typedef struct Cpu *Cpu;

extern T Cpu_new(MainMem mem);
extern void Cpu_free(T* cpu);

extern void fetch_decode_execute(T cpu);

#undef T
#endif
