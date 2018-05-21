#include <stdio.h>
#include <stdlib.h>
#include "smem.h"
#include "array.h"

struct integer {
    int value;
};

typedef struct integer Int;

Int* new_int(int value) {
    Int *integer;
    integer = (Int*) smalloc(sizeof(Int));
    integer->value = value;
    return integer;
}

int main()
{
    int i;
    Int *curr;

    Array *array1 = array_new(3);
    Array *array2 = array_new(0);
    Array *array3;

    array_add(array1, new_int(1));
    array_add(array1, new_int(2));
    array_add(array1, new_int(3));

    array_add(array2, new_int(1));
    array_add(array2, new_int(2));
    array_add(array2, new_int(3));

    Int *integer = array_get(array2, 2);
    free(integer);
    array_set(array2, 2, new_int(13));


    printf("array1\n");
    for (i = 0; i < array_size(array1); ++i)
    {
        curr = (Int*) array_get(array1, i);
        printf("%d\n", curr->value);
    }

    printf("array2\n");
    for (i = 0; i < array_size(array2); ++i)
    {
        curr = (Int*) array_get(array2, i);
        printf("%d\n", curr->value);
    }

    array_add_all(array1, array2);

    printf("array1 + array2\n");
    for (i = 0; i < array_size(array1); ++i)
    {
        curr = (Int*) array_get(array1, i);
        printf("%d\n", curr->value);
    }

    array3 = array_clone(array1);

    printf("array3\n");
    for (i = 0; i < array_size(array3); ++i)
    {
        curr = (Int*) array_get(array3, i);
        printf("%d\n", curr->value);
    }

    int size = array_size(array3);
    for (i = 0; i < size; ++i)
    {
        curr = (Int*) array_get(array3, 0);
        array_remove(array3, curr);
    }

    array_clear(array2);
    printf("array2\n");
    for (i = 0; i < array_size(array2); ++i)
    {
        curr = (Int*) array_get(array2, i);
        printf("%d\n", curr->value);
    }

    //free

    for (i = 0; i < array_size(array1); ++i)
    {
        curr = (Int*) array_get(array1, i);
        free(curr);
    }

    array_free(array1);
    array_free(array2);
    array_free(array3);

    return 0;
}
