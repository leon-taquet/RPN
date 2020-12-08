#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "parse.h"
#include "mainmem.h"
#include "machine.h"

/* void test_parse_instructions(FILE* fp)  */
/* { */
/*     Seq_T instructions = parse_instructions(fp); */

/*     for (int i = 0; i < Seq_length(instructions); ++i) { */
/*         fprintf(stdout, "%x", (uint32_t)(uintptr_t)Seq_get(instructions, i)); */
/*     } */

/*     Seq_free(&instructions); */
/* } */

/* void test_parse_opcode(FILE* fp) */
/* { */
/*     Seq_T instructions = parse_instructions(fp); */

/*     for (int i = 0; i < Seq_length(instructions); ++i) { */
/*         fprintf(stdout, "%d\n",  */
/*             parse_opcode((uint32_t)(uintptr_t)Seq_get(instructions, i))); */
/*     } */

/*     Seq_free(&instructions); */
/* } */

/* void test_three_register(FILE* fp) */
/* { */
/*     Seq_T instructions = parse_instructions(fp); */

/*     for (int i = 0; i < Seq_length(instructions); ++i) { */
/*         three_register t = parse_three_register( */
/*             (uint32_t)(uintptr_t)Seq_get(instructions, i) */
/*         ); */
/*         fprintf(stdout, "ra: %d rb: %d rc: %d\n",  */
/*             t.ra, t.rb, t.rc); */
/*     } */

/*     Seq_free(&instructions); */
/* } */

/* void test_load_value(FILE* fp) */
/* { */
/*     Seq_T instructions = parse_instructions(fp); */

/*     for (int i = 0; i < Seq_length(instructions); ++i) { */
/*         load_value l = parse_load_value( */
/*             (uint32_t)(uintptr_t)Seq_get(instructions, i) */
/*         ); */
/*         fprintf(stdout, "r: %d value: %d\n",  */
/*             l.r, l.value); */
/*     } */

/*     Seq_free(&instructions); */
/* } */

/* void test_mem_newfree(FILE* fp) */
/* { */
/*     MainMem mem = MainMem_new(parse_instructions(fp)); */

/*     MainMem_free(&mem); */
/*     assert(mem == NULL); */
/* } */

int main(int argc, char** argv) 
{
    assert(argc == 2);
    FILE* fp = fopen(argv[1], "rb");
    assert(fp != NULL);

    execute_program(fp); 

    fclose(fp);
    fp = NULL;

    return EXIT_SUCCESS;
}
