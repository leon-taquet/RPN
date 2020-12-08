#ifndef PARSE_INCLUDED
#define PARSE_INCLUDED

#include "mainmem.h"

#include <stdio.h>
#include <seq.h>
#include <stdint.h>

typedef enum Um_opcode {
    CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV, NAND, HALT,
    ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

typedef struct three_register {
    uint32_t ra, rb, rc;
} three_register;

typedef struct load_value {
    uint32_t r, value;
} load_value;

//extern Seq_T parse_instructions(FILE* fp);
extern seg_t *parse_instructions(FILE* fp);

extern Um_opcode parse_opcode(uint32_t instruction);

extern three_register parse_three_register(uint32_t instruction);

extern load_value parse_load_value(uint32_t instruction);
#endif
