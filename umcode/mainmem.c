#include "mainmem.h"

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define T MainMem

#define UNMAPPED_HINT 1000
#define SEGMENTS_HINT 1000


T MainMem_new(seg_t *zero_segment)
{
    T result = malloc(sizeof(*result));
    assert(result != NULL);
    
    result->unmapped = Seq_new(UNMAPPED_HINT);
    result->segments = Seq_new(SEGMENTS_HINT);

    Seq_addlo(result->segments, (void *)NULL);

    result->zero_seg = zero_segment;

    result->counter = 0;

    return result;
}

void MainMem_free(T* mem)
{
    assert(mem != NULL && *mem != NULL);

    Seq_free(&((*mem)->unmapped));

    int length = Seq_length((*mem)->segments);
    for (int i = 0; i < length; ++i) {
            seg_t *segment = (seg_t *)Seq_get((*mem)->segments, i);
        if (segment != NULL) {
                free(segment->seg);
                free(segment);
        }
    }
    free((*mem)->zero_seg->seg);
    free((*mem)->zero_seg);
    Seq_free(&((*mem)->segments));
    
    free(*mem);
    *mem = NULL;
}

void MainMem_unmap(T mem, uint32_t idx)
{
    seg_t *segment = Seq_put(mem->segments, idx, (void*)NULL);
    free(segment->seg);
    free(segment);

    Seq_addlo(mem->unmapped, (void*)(uintptr_t)idx);
}

uint32_t MainMem_map(T mem, uint32_t num)
{
    assert(~(uint32_t)0 > (uint32_t)Seq_length(mem->segments)
        || Seq_length(mem->unmapped) != 0);

    //Seq_T addition = Seq_new(num);
    seg_t *addition = malloc(sizeof(seg_t));
    uint32_t *add_seg = malloc(sizeof(uint32_t) * num);


    for (int i = 0; (uint32_t)i < num; ++i) {
            //Seq_addlo(addition, (void*)(uintptr_t)(uint32_t)0);
            add_seg[i] = (uint32_t)0;
    }

    addition->seg = add_seg;
    addition->size = num;

    if (Seq_length(mem->unmapped) > 0) {
        int idx = (uint32_t)(uintptr_t)Seq_remlo(mem->unmapped); 
        Seq_put(mem->segments, idx, (void*)addition);
        return idx;
    }

    Seq_addhi(mem->segments, (void*)addition);
    return Seq_length(mem->segments) - 1;
}

seg_t *get_segment(T mem, uint32_t idx)
{
    return (seg_t *)Seq_get(mem->segments, idx);
}

void replace_zero_segment(T mem, seg_t *replacement)
{
    //seg_t *old_zero = Seq_put(mem->segments, 0, replacement);
    seg_t *old_zero = mem->zero_seg;
    
    if (old_zero != NULL) {
        free(old_zero->seg);
        free(old_zero);
    }
    mem->zero_seg = replacement;
}

void MainMem_load_program(T mem, uint32_t segment_idx, 
                          uint32_t new_counter)
{
    if (segment_idx != (uint32_t)0) {
        seg_t *to_cpy = (seg_t *)Seq_get(mem->segments, segment_idx);
        int length = to_cpy->size; 
        seg_t *duplicate = malloc(sizeof(seg_t));
        uint32_t *dup_seg = malloc(sizeof(uint32_t) * length);
        for (int i = 0; i < length; ++i) {
            //Seq_addhi(duplicate, Seq_get(to_cpy, i));
            dup_seg[i] = to_cpy->seg[i];                
        }
        duplicate->seg = dup_seg;
        duplicate->size = length;

        replace_zero_segment(mem, duplicate);
    }

    mem->counter = new_counter;
}

void MainMem_write(T mem, uint32_t segment_idx, uint32_t word_idx, 
    uint32_t val) 
{
//    Seq_put((Seq_T)Seq_get(mem->segments, segment_idx), word_idx,
//        (void*)(uintptr_t)val);
    if (segment_idx != 0){
        seg_t *segment = (seg_t *)Seq_get(mem->segments, segment_idx);
        segment->seg[word_idx] = val;
    }
    else{
        mem->zero_seg->seg[word_idx] = val;
    }
    
}


uint32_t MainMem_read(T mem, uint32_t segment_idx, uint32_t word_idx)
{
//    return (uint32_t)(uintptr_t)Seq_get(
//        (Seq_T)Seq_get(mem->segments, segment_idx), word_idx
//    );
    if (segment_idx != 0){
        seg_t *segment = (seg_t *)Seq_get(mem->segments, segment_idx);
        return segment->seg[word_idx];
    }
    else{
        return mem->zero_seg->seg[word_idx];
    }
}

uint32_t MainMem_next_instruction(T mem) 
{
    //return MainMem_read(mem, 0, mem->counter++);
    return mem->zero_seg->seg[mem->counter++];
}
