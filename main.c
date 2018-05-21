#include <stdio.h>
#include <stdlib.h>
#include "smem.h"
#include "array.h"
#include "birch.h"

int main()
{
    double x[] = {1, 2, 3};
    Entry* entry = new_entry(x, 3, 0);
    return 0;
}
