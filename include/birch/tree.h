#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include "entry.h"

struct tree
{
    struct node* root;
    struct node* leaf_list;
    int instance_index;
    int dimensionality;
};
typedef struct tree Tree;

int* tree_get_cluster_id_by_instance_index(Tree* tree);
Tree* tree_create(int dimensionality, int branching_factor, double threshold, double (*distance)(Entry*, Entry*), bool apply_merging_refinement);
int tree_insert(Tree* tree, double* sample);
void tree_free(Tree* tree);

#endif
