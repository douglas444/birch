#include "../../include/util/integer.h"
#include "../../include/util/smem.h"


Integer* integer_new(int value)
{
    Integer* integer = (Integer*) smalloc(sizeof(Integer));
    integer->value = value;
    return integer;
}


bool integer_array_cmp(Array* a1, Array* a2)
{
    for (int i = 0; i < array_size(a1); ++i)
    {
        if (((Integer*) array_get(a1, i))->value
            != ((Integer*) array_get(a2, i))->value)
        {
            return false;
        }
    }
    return true;
}
