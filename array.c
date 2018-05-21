#include <stdio.h>
#include <stdlib.h>
#include "smem.h"
#include "array.h"

struct array
{
    void **backing;
    size_t used;
    size_t size;
};

Array* array_new(size_t initial_size)
{
    Array *array = (Array*) smalloc(sizeof(Array));
    array->backing = smalloc(sizeof(void*) * initial_size);
    array->size = initial_size;
    array->used = 0;
    return array;
}

void array_add(Array *array, void *data)
{
    if (array->size == array->used)
    {
        if (array->size == 0)
        {
            array->size = 1;
        }

        array->size *= 2;
        array->backing = srealloc(array->backing, array->size * sizeof(void*));
    }

    array->backing[array->used++] = data;
}

void array_add_all(Array *dest, Array *src)
{
    int i;

    if (dest->used + src->used > dest->size)
    {
        dest->size = dest->used + src->used;
        dest->backing = srealloc(dest->backing, (dest->used + src->used) * sizeof(void*));
    }

    for (i = 0; i < src->used; ++i)
    {
        dest->backing[dest->used++] = array_get(src, i);
    }
}

Array* array_clone(Array *array)
{
    Array *clone = (Array*) smalloc(sizeof(Array));
    clone->backing = smalloc(sizeof(void*) * array->size);
    clone->backing = smemcpy(clone->backing, array->backing, array->used * sizeof(void*));
    clone->size = array->size;
    clone->used = array->used;

    return clone;
}

void array_free(Array *array)
{
    free(array->backing);
    free(array);
}

size_t array_size(Array *array)
{
    return array->used;
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
    else
    {
        printf("ERROR: array.c/get_array(): \"Array index out of bounds\"\n");
        exit(1);
    }
}

void array_remove(Array *array, void *data)
{
    int i;

    i = 0;
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
        ++i;
    }

    array->used--;
}

void* array_remove_by_index(Array *array, size_t index)
{
    void* data;

    if (index >= array->used)
    {
        printf("ERROR: array.c/get_array(): \"Array index out of bounds\"\n");
        exit(1);
    }

    data = array->backing[index];

    while (index < array->used - 1)
    {
        array->backing[index] = array->backing[index + 1];
        ++index;
    }

    array->used--;
    return data;
}

void array_clear(Array *array)
{
    int i;
    int size;

    size = array->used;
    for (i = 0; i < size; ++i)
    {
        array_remove_by_index(array, 0);
    }
}
