#include "cpu.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "parse.h"

#define N_REGISTERS 8

#define T Cpu

static const uint32_t MASK_VALUE = (~(uint32_t)0 >> 7);
static const uint32_t MASK_REG = ~(~(uint32_t)0 << 3);  
static const uint32_t MASK_RC = MASK_REG;
static const uint32_t MASK_RB = MASK_RC << 3;
static const uint32_t MASK_RA = MASK_RB << 3; 

uint32_t registers[N_REGISTERS];


struct Cpu {
        //uint32_t registers[N_REGISTERS];
    MainMem mem;
};


T Cpu_new(MainMem mem)
{
    T result = malloc(sizeof(*result));
    result->mem = mem;

    for (int i = 0; i < N_REGISTERS; ++i) {
        registers[i] = (uint32_t)0;
    }

    return result;
}

void Cpu_free(T* cpu)
{
    MainMem_free(&((*cpu)->mem));

    free(*cpu);
    *cpu = NULL;
}

void op_output(uint32_t instruction)
{
    printf("%c", (char)registers[ (instruction & MASK_RC)]);
    
}

void op_map(T cpu, uint32_t instruction)
{
    registers[(instruction & MASK_RB) >> 3] = MainMem_map(cpu->mem, 
                                       registers[ (instruction & MASK_RC)]);
}

void op_unmap(T cpu, uint32_t instruction)
{
    MainMem_unmap(cpu->mem, registers[ (instruction & MASK_RC)]);
}

void op_loadvalue(uint32_t instruction)
{
    registers[(instruction >> 25) & MASK_REG] = 
        (uint32_t)instruction & MASK_VALUE;
}

void op_cmov(uint32_t instruction)
{
    if (registers[ (instruction & MASK_RC)] != (uint32_t)0) {
        registers[(instruction & MASK_RA) >> 6] = 
            registers[(instruction & MASK_RB) >> 3];
    }
}

void op_sload(T cpu, uint32_t instruction)
{
    registers[(instruction & MASK_RA) >> 6] = MainMem_read(cpu->mem, 
                                    registers[(instruction & MASK_RB) >> 3],
                                    registers[ (instruction & MASK_RC)]);
}

void op_sstore(T cpu, uint32_t instruction)
{
    MainMem_write(cpu->mem, 
                  registers[(instruction & MASK_RA) >> 6], 
                  registers[(instruction & MASK_RB) >> 3], 
                  registers[ (instruction & MASK_RC)]);
}

void op_add(uint32_t instruction)
{
    registers[(instruction & MASK_RA) >> 6] = 
        registers[(instruction & MASK_RB) >> 3] 
        + registers[(instruction & MASK_RC)];
}   


void op_multiply(uint32_t instruction)
{
    registers[(instruction & MASK_RA) >> 6] = 
        registers[(instruction & MASK_RB) >> 3] 
        * registers[(instruction & MASK_RC)];
}   

void op_divide(uint32_t instruction)
{
    registers[(instruction & MASK_RA) >> 6] = 
        registers[(instruction & MASK_RB) >> 3] 
        / registers[(instruction & MASK_RC)];
}   

void op_nand(uint32_t instruction)
{
    registers[(instruction & MASK_RA) >> 6] = 
        ~(registers[(instruction & MASK_RB) >> 3]
          & registers[(instruction & MASK_RC)]);
}

void op_input(uint32_t instruction)
{
    uint32_t result = ~(uint32_t)0;
    int input = getc(stdin);
    if (input != EOF) {
        result = (uint32_t)(char)input;
    }
    registers[(instruction & MASK_RC)] = result;
}

void op_load_program(T cpu, uint32_t instruction)
{
    MainMem_load_program(cpu->mem, registers[(instruction & MASK_RB) >> 3],
                                   registers[(instruction & MASK_RC)]);
}

char *OP_E(Um_opcode OP)
{
        switch (OP) {
        case OUT:
            return "OUT";
            break; 
        case IN:
            return "IN";
            break;
        case ACTIVATE:
            return "ACTIVATE";
            break; 
        case INACTIVATE:
            return "INACTIVATE";
            break;
        case LV:
            return "Load Value";
            break;
        case CMOV:
            return "CMOV";
            break;
        case SLOAD:
            return "SLOAD";
            break;
        case SSTORE:
            return "SSTORE";
            break;
        case ADD:
            return "ADD";
            break;
        case MUL:
            return "MUL";
            break;
        case DIV:
            return "DIV";
            break;
        case NAND:
            return "NAND";
            break;
        case LOADP:
            return "Load Program";            
            break;
        case HALT:
            return "HALTING";
            break;
        }
        return "No OP";
}

void fetch_decode_execute(T cpu)
{
    bool running = true;
    bool trace = getenv("UMTRACE") != NULL;
    MainMem mem = cpu->mem;

    uint32_t *zero_seg = mem->zero_seg->seg;

    while ( running ){
        
        uint32_t instruction = zero_seg[mem->counter++];

        if (trace) {
            if ((Um_opcode)(instruction >> 28) == LV){
                fprintf(stderr, "Loading Value...\n");
            }
            else{
                fprintf(stderr, "OP: %s r%d = %d, r%d = %d, r%d = %d)\n",
                        OP_E((Um_opcode)(instruction >> 28)),
                        (instruction & MASK_RA) >> 6,
                        registers[(instruction & MASK_RA) >> 6] , 
                        (instruction & MASK_RB) >> 3,
                        registers[(instruction & MASK_RB) >> 3],
                                 (instruction & MASK_RC), 
                                  registers[(instruction & MASK_RC)]);
            }
            
        }

        switch ((Um_opcode)(instruction >> 28)) {
        case OUT:
            op_output(instruction);
            break; 
        case IN:
            op_input(instruction);
            break;
        case ACTIVATE:
            op_map(cpu, instruction);
            break; 
        case INACTIVATE:
            op_unmap(cpu, instruction);
            break;
        case LV:
            op_loadvalue(instruction);
            break;
        case CMOV:
            op_cmov(instruction);
            break;
        case SLOAD:
            //op_sload(cpu, instruction);
            registers[(instruction & MASK_RA) >> 6] = 
                MainMem_read(cpu->mem, 
                             registers[(instruction & MASK_RB) >> 3],
                             registers[ (instruction & MASK_RC)]);
            break;
        case SSTORE:
            //op_sstore(cpu, instruction);
            MainMem_write(cpu->mem, 
                          registers[(instruction & MASK_RA) >> 6], 
                          registers[(instruction & MASK_RB) >> 3], 
                          registers[ (instruction & MASK_RC)]);
            break;
        case ADD:
            op_add(instruction);
            break;
        case MUL:
            op_multiply(instruction);
            break;
        case DIV:
            op_divide(instruction);
            break;
        case NAND:
            op_nand(instruction);
            break;
        case LOADP:
            op_load_program(cpu, instruction);
            zero_seg = mem->zero_seg->seg;
            break;
        case HALT:
            running = false;
        }
    };
}

/********************************************************/
/* void test_output()                                   */
/* {                                                    */
/*     MainMem mem = MainMem_new(NULL);                 */
/*     Cpu cpu = Cpu_new(mem);                          */
/*                                                      */
/*     registers[0] = '\n';                             */
/*     op_output(cpu, (three_register){ 0, 0, 0 });     */
/*                                                      */
/*     for (int i = 0; i < N_REGISTERS; ++i)            */
/*     {                                                */
/*         registers[i] = 'a' + i;                      */
/*         op_output(cpu, (three_register){ i, 0, 0 }); */
/*     }                                                */
/*                                                      */
/*     Cpu_free(&cpu);                                  */
/* }                                                    */
/********************************************************/
