#include <stdlib.h>

void* smalloc(size_t mem_size);
void* srealloc(void *mem_pos, size_t mem_size);
void* smemcpy(void *mem_pos_dest, void *mem_pos_src, size_t mem_size);
