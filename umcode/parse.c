#include "parse.h"

#include <stdlib.h>
#include <bitpack.h>

#define LENGTH_HINT 100

static const uint32_t MASK_VALUE = (~(uint32_t)0 >> 7);
static const uint32_t MASK_REG = ~(~(uint32_t)0 << 3);  
static const uint32_t MASK_RC = MASK_REG;
static const uint32_t MASK_RB = MASK_RC << 3;
static const uint32_t MASK_RA = MASK_RB << 3; 


//Seq_T parse_instructions(FILE* fp) 
seg_t *parse_instructions(FILE* fp) 
{

    Seq_T result = Seq_new(LENGTH_HINT);

    int currentInt = fgetc(fp);

    while (currentInt != EOF) {
        uint8_t current = (uint8_t)currentInt;

        uint64_t instruction = 0;
        instruction = Bitpack_newu(instruction,  
            8 * sizeof(uint8_t), 24, (uint64_t)current);
        for (int i = 16; i >= 0; i -= 8) {
            current = (uint8_t)fgetc(fp);
            instruction = Bitpack_newu(instruction, 
                8 * sizeof(uint8_t), i, (uint64_t)current);
        }

        uint32_t intermediate = (uint32_t)instruction;
        
        Seq_addhi(result, (void*)(uintptr_t)intermediate);

        currentInt = fgetc(fp);
    }

    int length = Seq_length(result);
    uint32_t *r = malloc(sizeof(uint32_t)*length); 

    for (int i = 0; i < length; ++i) {                 
            r[i] = (uint32_t)(uintptr_t)Seq_get(result, i);
    }

    seg_t *zero_seg = malloc(sizeof(seg_t));

    zero_seg->seg = r;
    zero_seg->size = length;

    Seq_free(&result);
    return zero_seg;
}

Um_opcode parse_opcode(uint32_t instruction)
{
    return (Um_opcode)(instruction >> 28);
    //return (Um_opcode)Bitpack_getu((uint64_t)instruction, 4, 28);
}

three_register parse_three_register(uint32_t instruction) 
{
    three_register result;
    /* result.ra = (uint32_t)Bitpack_getu((uint64_t)instruction, 3, 6); */
    /* result.rb = (uint32_t)Bitpack_getu((uint64_t)instruction, 3, 3); */
    /* result.rc = (uint32_t)Bitpack_getu((uint64_t)instruction, 3, 0); */
    result.ra = (instruction & MASK_RA) >> 6;
    result.rb = (instruction & MASK_RB) >> 3; 
    result.rc = (instruction & MASK_RC);

    return result;
}

load_value parse_load_value(uint32_t instruction)
{
    load_value result;
    //result.value = (uint32_t)Bitpack_getu((uint64_t)instruction, 25, 0);
    result.value = instruction & MASK_VALUE;
    result.r = (instruction >> 25) & MASK_REG;
    //result.r = (uint32_t)Bitpack_getu((uint64_t)instruction, 3, 25);

    return result;
}
