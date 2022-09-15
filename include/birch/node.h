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
    bool apply_node_merging_refinement;
};
typedef struct node Node;

void node_free(Node *node);
Node* node_create(int branching_factor, double threshold, double (*distance)(struct entry*, struct entry*), bool is_leaf, bool apply_node_merging_refinement);
bool node_is_dummy(Node* node);
Entry* node_find_closest_entry(Node *node, Entry* entry);
int node_find_closest_subcluster(Node* node, Entry* entry);
void node_redistribute_entries(Node* node, Array* old_entries, Pair *far_entries, Entry* new_entry_1, Entry* new_entry_2);
void node_redistribute_entries_double(Node* node, Array* old_entries_1, Array* old_entries_2, Pair* close_entries, Entry* new_entry_1, Entry* new_entry_2);
void node_redistribute_entries_merge(Node *node, Array* old_entries_1, Array* old_entries_2, Entry* new_entry);
Pair* node_split_entry(Node* node, Entry* closest_entry);
void node_replace_closest_pair_with_new_entries(Node *node, Pair* pair, Entry* new_entry_1, Entry* new_entry_2);
void node_replace_closest_pair_with_new_merged_entry(Node* node, Pair* pair, Entry* new_entry);
bool node_insert_entry(Node* node, Entry* entry, bool* hold_memory);

#endif
