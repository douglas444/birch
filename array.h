#include <stdlib.h>

typedef struct array Array;

void array_init(size_t unit, size_t initial_size);
void array_add(Array *array, void *data);
void array_free(Array *array);
void* array_get(Array *array, size_t index);
void array_set(Array *array, size_t index, void *data);
void array_remove(Array *array, void *data);
void array_remove_by_index(Array *array, size_t index);
void array_add_all(Array *dest, Array *src);
Array* array_clone(Array *array);
