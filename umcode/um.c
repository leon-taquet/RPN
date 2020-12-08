#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "parse.h"
#include "mainmem.h"
#include "machine.h"



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
