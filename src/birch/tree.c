#include <stdbool.h>
#include "../../include/birch/tree.h"
#include "../../include/birch/node.h"
#include "../../include/util/smem.h"
#include "../../include/util/integer.h"


void tree_split_root(Tree *tree);


Tree* tree_create
(
    int dimensionality,
    int branching_factor,
    double threshold,
    double (*distance)(struct entry*, struct entry*),
    bool apply_merging_refinement
)
{
    Tree* tree = (Tree*) smalloc(sizeof(Tree));

    tree->dimensionality = dimensionality;
    tree->root = node_create(branching_factor, threshold, distance, true, apply_merging_refinement);
    tree->leaf_list = node_create(branching_factor, threshold, distance, true, apply_merging_refinement);
    tree->leaf_list->next_leaf = tree->root;
    tree->instance_index = 0;

    return tree;
}


void tree_free_rec(Node* root)
{
    int i;
    int entries_size;
    Entry *curr_entry;

    entries_size = array_size(root->entries);

    for (i = 0; i < entries_size; ++i)
    {
        curr_entry = (Entry*) array_get(root->entries, i);
        if (curr_entry->child != NULL)
        {
            tree_free_rec(curr_entry->child);
        }

        if (curr_entry->indexes != NULL)
        {
            array_deep_clear(curr_entry->indexes);
        }
        entry_free(curr_entry);
    }

    node_free(root);
}


void tree_free(Tree* tree)
{
    tree_free_rec(tree->root);
    node_free(tree->leaf_list);
    free(tree);
}


int tree_insert(Tree* tree, double* sample)
{
    int instance_index = tree->instance_index;
    tree->instance_index++;

    Entry* entry = entry_create(sample, tree->dimensionality, instance_index);
    bool hold_memory = false;
    bool dont_split = node_insert_entry(tree->root, entry, &hold_memory);

    if (dont_split == false)
    {
        // if dontSplit is false, it means there was not enough space to insert the new entry in the tree,
        // therefore wee need to split the root to make more room
        tree_split_root(tree);
    }

    if (hold_memory == false)
    {
        entry_free(entry);
    }

    return instance_index;
}


void tree_split_root(Tree *tree)
{
    // the split happens by finding the two entries in this node that are the most far apart
    // we then use these two entries as a "pivot" to redistribute the old entries into two new nodes

    Pair* pair;
    Entry* new_entry_1;
    Entry* new_entry_2;
    Node* new_node_1;
    Node* new_node_2;
    Node* new_root;

    pair = pair_find_farthest(tree->root->entries, tree->root->distance);

    new_entry_1 = entry_create_default(pair->e1->dim);
    new_node_1 = node_create(tree->root->branching_factor,
                             tree->root->threshold,
                             tree->root->distance,
                             tree->root->is_leaf,
                             tree->root->apply_merging_refinement);
    new_entry_1->child = new_node_1;

    new_entry_2 = entry_create_default(pair->e1->dim);
    new_node_2 = node_create(tree->root->branching_factor,
                             tree->root->threshold, tree->root->distance,
                             tree->root->is_leaf,
                             tree->root->apply_merging_refinement);
    new_entry_2->child = new_node_2;

    // the new root that hosts the new entries
    new_root = node_create(tree->root->branching_factor,
                           tree->root->threshold,
                           tree->root->distance,
                           false,
                           tree->root->apply_merging_refinement);
    array_add(new_root->entries, new_entry_1);
    array_add(new_root->entries, new_entry_2);

    // this updates the pointers to the list of leaves
    if(tree->root->is_leaf == true)
    {
        // if root was a leaf
        tree->leaf_list->next_leaf = new_node_1;
        new_node_1->prev_leaf = tree->leaf_list;
        new_node_1->next_leaf = new_node_2;
        new_node_2->prev_leaf = new_node_1;
    }

    // redistributes the entries in the root between newEntry1 and newEntry2
    // according to the distance to p.e1 and p.e2
    node_redistribute_entries(tree->root, tree->root->entries, pair, new_entry_1, new_entry_2);

    // updates the root
    free(pair);
    node_free(tree->root);
    tree->root = new_root;
}


Array* tree_get_subclusters(Tree* tree)
{
    Array* subclusters = array_create(1);
    Node* leaf = tree->leaf_list->next_leaf; // the first leaf is dummy!

    while(leaf != NULL)
    {
        if(!node_is_dummy(leaf))
        {

            for (int i = 0; i < array_size(leaf->entries); ++i)
            {
                Entry* entry = (Entry*) array_get(leaf->entries, i);
                array_add(subclusters, array_clone(entry->indexes));
            }
        }
        leaf = leaf->next_leaf;
    }

    return subclusters;
}


int tree_count_subclusters(Tree* tree)
{
    int count = 0;
    Node* leaf = tree->leaf_list->next_leaf; // the first leaf is dummy!

    while(leaf != NULL)
    {
        if(!node_is_dummy(leaf))
        {
            for (int i = 0; i < array_size(leaf->entries); ++i)
            {
                Entry* entry = (Entry*) array_get(leaf->entries, i);
                for (int j = 0; j < array_size(entry->indexes); ++j)
                {
                    ++count;
                }
            }
        }
        leaf = leaf->next_leaf;
    }

    return count;
}


int* tree_get_cluster_id_by_instance_index(Tree* tree)
{
    Node* leaf = tree->leaf_list->next_leaf; // the first leaf is dummy!
    int* cluster_id_by_entry_index = smalloc(tree->instance_index * sizeof(int));
    int cluster_id = 0;

    while(leaf != NULL)
    {
        if(!node_is_dummy(leaf))
        {
            for (int i = 0; i < array_size(leaf->entries); ++i)
            {
                Entry* entry = (Entry*) array_get(leaf->entries, i);
                for (int j = 0; j < array_size(entry->indexes); ++j)
                {
                    Integer* index = (Integer*) array_get(entry->indexes, j);
                    cluster_id_by_entry_index[index->value] = cluster_id;
                }
                ++cluster_id;
            }
        }
        leaf = leaf->next_leaf;
    }

    return cluster_id_by_entry_index;
}
