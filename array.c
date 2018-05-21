#include <stdio.h>
#include <stdlib.h>
#include "smem.h"
#include "array.h"

struct array
{
    void **backing;
    int used;
    int size;
    size_t unit;
};

void array_init(size_t unit, size_t initial_size)
{
    Array *array = (Array*) smalloc(sizeof(Array));
    array->backing = smalloc(unit * initial_size);
    array->size = initial_size;
    array->used = 0;
    array->unit = unit;
}

void array_add(Array *array, void *data)
{
    if (array->size == array->used)
    {
        array->size *= 2;
        array->backing = srealloc(array->backing, array->size * array->unit);
    }

    array->backing[array->used++] = data;
}

void array_add_all(Array *dest, Array *src)
{
    int i;

    if (dest->used + src->used > dest->size)
    {
        dest->size = dest->used + src->used;
        dest->backing = srealloc(dest->backing, dest->used + src->used);
    }

    for (i = 0; i < src->used; ++i)
    {
        dest->backing[dest->used++] = array_get(src, i);
    }
}

Array* array_clone(Array *array)
{
    Array *clone = (Array*) smalloc(sizeof(Array));
    clone->backing = smemcpy(clone->backing, array->backing, array->used * array->unit);
    clone->size = array->size;
    clone->used = array->used;
    clone->unit = array->unit;

    return clone;
}

void array_free(Array *array)
{
    free(array->backing);
}

void* array_get(Array *array, size_t index)
{
    if (index < array->used)
    {
        return array->backing[index];
    }
    printf("ERROR: array.c/get_array(): \"Array index out of bounds\"\n");
    exit(1);
}

void array_set(Array *array, size_t index, void *data)
{
    if (index < array->used)
    {
        array->backing[index] = data;
    }
    printf("ERROR: array.c/get_array(): \"Array index out of bounds\"\n");
    exit(1);
}

void array_remove(Array *array, void *data)
{
    int i;

    while (i < array->size && array->backing[i] != data)
    {
        ++i;
    }

    if (i >= array->size)
    {
        return;
    }

    while (i < array->used - 1)
    {
        array->backing[i] = array->backing[i + 1];
    }

    array->used--;
}

void array_remove_by_index(Array *array, size_t index)
{

    while (index < array->used - 1)
    {
        array->backing[index] = array->backing[index + 1];
    }

    array->used--;
}
