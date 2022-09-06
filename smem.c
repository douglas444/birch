#include <stdlib.h>
#include <stdio.h>
#include "smem.h"
#include <string.h>

void* smalloc(size_t mem_size)
/* ====================================
 * Allocates memory
 * ====================================
 */
{
    void *mem_pos = (void*) malloc(mem_size);

    if (mem_pos == NULL)
    {
        printf("ERROR: array.c/smalloc(): \"Unable to allocate memory\"\n");
        exit(1);
    }
    else
    {
        return mem_pos;
    }
}

void* scalloc(int num, size_t elem_size)
/* ====================================
 * Allocates memory
 * ====================================
 */
{
    void *mem_pos = (void*) calloc(num, elem_size);

    if (mem_pos == NULL)
    {
        printf("ERROR: array.c/cmalloc(): \"Unable to allocate memory\"\n");
        exit(1);
    }
    else
    {
        return mem_pos;
    }
}

void* srealloc(void *mem_pos, size_t mem_size)
/* ====================================
 * Reallocates memory
 * ====================================
 */
{
    mem_pos = realloc(mem_pos, mem_size);

    if (mem_pos == NULL)
    {
        printf("ERROR: array.c/srealloc(): \"Unable to reallocate memory\"\n");
        exit(1);
    }
    else
    {
        return mem_pos;
    }
}

void* smemcpy(void *mem_pos_dest, void *mem_pos_src, size_t mem_size)
{
    return memcpy(mem_pos_dest, mem_pos_src, mem_size);
}
