#ifndef PAIR_H
#define PAIR_H

#include <stdlib.h>
#include "./entry.h"

struct pair
{
    struct entry* e1;
    struct entry* e2;
};
typedef struct pair Pair;

Pair* pair_create_default();
Pair* pair_create(Entry* e1, Entry* e2);
bool pair_cmp(Pair* p1, Pair* p2);
Pair* pair_find_farthest(Array* entries, double (*distance)(struct entry*, struct entry*));
Pair* pair_find_closest(Array* entries, double (*distance)(struct entry*, struct entry*));

#endif
