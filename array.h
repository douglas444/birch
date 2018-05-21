#include <stdlib.h>

struct integer {
    int value;
};

typedef struct array Array;
typedef struct integer Integer;

Array* array_new(size_t initial_size);
void array_add(Array *array, void *data);
void array_free(Array *array);
void* array_get(Array *array, size_t index);
void array_set(Array *array, size_t index, void *data);
void array_remove(Array *array, void *data);
void* array_remove_by_index(Array *array, size_t index);
void array_add_all(Array *dest, Array *src);
Array* array_clone(Array *array);
size_t array_size(Array *array);
void array_clear(Array *array);

Integer* new_integer(int value);

