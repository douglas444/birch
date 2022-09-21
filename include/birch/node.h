#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include "./pair.h"
#include "../util/array.h"

struct node
{
    Array *entries;
    int branching_factor;
    double threshold;
    bool is_leaf;
    double (*distance)(struct entry*, struct entry*);
    struct node *next_leaf;
    struct node *prev_leaf;
    bool apply_merging_refinement;
};
typedef struct node Node;

void node_free(Node *node);
Node* node_create(int branching_factor, double threshold, double (*distance)(struct entry*, struct entry*), bool is_leaf, bool apply_merging_refinement);
bool node_is_dummy(Node* node);
void node_redistribute_entries(Node* node, Array* old_entries, Pair *far_entries, Entry* new_entry_1, Entry* new_entry_2);
bool node_insert_entry(Node* node, Entry* entry, bool* hold_memory);

#endif
