#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "../../include/birch/pair.h"
#include "../../include/util/smem.h"


Pair* pair_create_default()
{
    return (Pair*) smalloc(sizeof(Pair));
}


Pair* pair_create(Entry* e1, Entry* e2)
{
    Pair* pair = pair_create_default();
    pair->e1 = e1;
    pair->e2 = e2;
    return pair;
}


bool pair_cmp(Pair* p1, Pair* p2)
{
    if (entry_cmp(p1->e1, p2->e1) == true
        && entry_cmp(p1->e2, p2->e2) == true)
    {
        return true;
    }

    if (entry_cmp(p1->e1, p2->e2) == true
        && entry_cmp(p1->e2, p2->e1) == true)
    {
        return true;
    }

    return false;
}


Pair* pair_find_farthest
(
    Array* entries,
    double (*distance)(struct entry*, struct entry*)
)
{
    int i, j;
    double max_dist;
    double curr_dist;
    Entry* e1;
    Entry* e2;
    Pair* pair;

    if (array_size(entries) < 2)
    {
        return NULL;
    }

    pair = pair_create_default();
    max_dist = -1;

    for (i = 0; i < array_size(entries) - 1; ++i)
    {
        for (j = i + 1; j < array_size(entries); ++j)
        {
            e1 = (Entry*) array_get(entries, i);
            e2 = (Entry*) array_get(entries, j);

            curr_dist = distance(e1, e2);

            if(curr_dist > max_dist)
            {
                pair->e1 = e1;
                pair->e2 = e2;
                max_dist = curr_dist;
            }
        }
    }

    return pair;
}


Pair* pair_find_closest
(
    Array* entries,
    double (*distance)(struct entry*, struct entry*)
)
{
    int i, j;
    double min_dist;
    double curr_dist;
    Entry* e1;
    Entry* e2;
    Pair* pair;

    if (array_size(entries) < 2)
    {
        return NULL;
    }

    pair = pair_create_default();
    min_dist = DBL_MAX;

    for (i = 0; i < array_size(entries) - 1; ++i)
    {
        for (j = i + 1; j < array_size(entries); ++j)
        {
            e1 = (Entry*) array_get(entries, i);
            e2 = (Entry*) array_get(entries, j);

            curr_dist = distance(e1, e2);

            if(curr_dist < min_dist)
            {
                pair->e1 = e1;
                pair->e2 = e2;
                min_dist = curr_dist;
            }
        }
    }

    return pair;
}
