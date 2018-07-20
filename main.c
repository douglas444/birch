#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "smem.h"
#include "array.h"
#include "birch.h"

double distance(Entry* entry_1, Entry* entry_2)
{
    return 5;
}

int main()
{
    srand((unsigned) time(NULL));

    int i;
    double x[] = {1, 2};
    Tree* tree;
    Entry* entry;

    tree = create_tree(2, 2, distance, false);

    for (i = 0; i < 10000; ++i)
    {
        x[0] = rand();
        x[1] = rand();
        entry = create_entry(x, 2, 0);
        insert_entry_in_tree(tree, entry);
    }

    free_tree(tree);

    return 0;
}
