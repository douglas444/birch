#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "birch.h"
#include "array.h"
#include "smem.h"

#define INDEXES_INITIAL_SIZE 4

struct entry
{
    int dim;
    int n;
    double* ls;
    double* ss;
    struct node* child;
    Array* indexes;
    int subcluster_id;
};

struct pentry
{
    struct entry* e1;
    struct entry* e2;
};

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

struct tree
{
    struct node* root;
    struct node* leaf_list;
    int instance_index;
    bool automatic_rebuild;
    long memory_limit;
    long periodic_limit_check;
};

typedef struct entry Entry;
typedef struct node Node;
typedef struct pentry PEntry;
typedef struct tree Tree;

Entry* create_default_entry()
{
    Entry* entry = (Entry*) smalloc(sizeof(Entry));

    entry->dim = 0;
    entry->n = 0;
    entry->ls = NULL;
    entry->ss = NULL;
    entry->child = NULL;
    entry->indexes = NULL;
    entry->subcluster_id = -1;

    return entry;
}

Entry* create_entry
(
    double* x,
    int dim,
    int index
)
{
    int i;

    Entry* entry = create_default_entry();

    entry->n = 1;
    entry->dim = dim;

    entry->ls = (double*) smalloc(sizeof(double));
    smemcpy(entry->ls, x, entry->dim);

    entry->ss = (double*) smalloc(sizeof(double));
    for (i = 0; i < entry->dim; i++)
    {
        entry->ss[i] = entry->ls[i] * entry->ls[i];
    }

    entry->indexes = array_new(INDEXES_INITIAL_SIZE);
    array_add(entry->indexes, new_integer(index));

    return entry;

}

void free_entry(Entry *entry)
{
    entry->dim = 0;
    entry->n = 0;
    if (entry->ls != NULL)
    {
        free(entry->ls);
    }
    if (entry->ss != NULL)
    {
        free(entry->ss);
    }
    array_free(entry->indexes);
    free(entry);

}

void update_entry(Entry *e1, Entry *e2)
{
    int i;

    e1->n += e2->n;

    if (e1->ls == NULL)
    {
        e1->ls = (double*) smemcpy(e1->ls, e2->ls, e2->dim);
    }
    else
    {
        for(i = 0; i < e1->dim; ++i)
        {
            e1->ls[i] += e2->ls[i];
        }
    }

    if(e1->ss == NULL)
    {
        e1->ss = (double*) smemcpy(e1->ss, e2->ss, e2->dim);
    }
    else
    {
        for(i = 0; i < e1->dim; i++)
        {
            e1->ss[i] += e2->ss[i];
        }
    }

    if(e1->child == NULL)
    {
        if(e1->indexes != NULL && e2->indexes != NULL)
        {
            array_add_all(e1->indexes, e2->indexes);
        }
        else if(e1->indexes == NULL && e2->indexes != NULL)
        {
            e1->indexes = array_clone(e2->indexes);
        }
    }
}

bool is_within_threshold
(
    Entry *e1,
    Entry *e2,
    double threshold,
    double (*distance)(Entry*, Entry*)
)
{
    double dist = distance(e1, e2);

    if(dist == 0 || dist <= threshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Node* create_node
(
    int branching_factor,
    double threshold,
    double (*distance)(struct entry*, struct entry*),
    bool is_leaf,
    bool apply_merging_refinement
)
{
    Node* node = (Node*) smalloc(sizeof(Node));
    node->branching_factor = branching_factor;
    node->threshold = threshold;
    node->distance = distance;
    node->entries = array_new(branching_factor);
    node->is_leaf = is_leaf;
    node->next_leaf = NULL;
    node->prev_leaf = NULL;
    node->apply_merging_refinement = apply_merging_refinement;
    return node;
}

bool is_dummy(Node* node)
{
    if ((node->prev_leaf != NULL || node->next_leaf != NULL) &&
        node->branching_factor == 0 && node->threshold == 0 &&
        array_size(node->entries))
    {
        return true;
    }
    else
    {
        return false;
    }
}

PEntry* create_default_pentry()
{
    return (PEntry*) smalloc(sizeof(PEntry));
}

PEntry* create_pentry(Entry* e1, Entry* e2)
{
    PEntry* pentry = create_default_pentry();
    pentry->e1 = e1;
    pentry->e2 = e2;
    return pentry;
}

bool pentry_cmp(PEntry* p1, PEntry* p2)
{
    if (p1->e1 == p2->e1 && p1->e2 == p1->e2)
    {
        return true;
    }

    if (p1->e1 == p2->e2 && p1->e2 == p1->e1)
    {
        return true;
    }

    return false;
}

Entry* find_closest_entry(Node *node, Entry* entry)
{
    int i;
    double min_dist;
    double curr_dist;
    Entry* curr_entry;
    Entry* closest_entry;

    closest_entry = NULL;
    min_dist = DBL_MAX;

    for (i = 0; i < array_size(node->entries); ++i)
    {
        curr_entry = array_get(node->entries, i);
        curr_dist = node->distance(curr_entry, entry);

        if (curr_dist < min_dist)
        {
            min_dist = curr_dist;
            closest_entry = curr_entry;
        }
    }

    return closest_entry;
}

PEntry* find_farthest_entry_pair(Array* entries, double (*distance)(struct entry*, struct entry*))
{
    int i, j;
    double max_dist;
    double curr_dist;
    Entry* e1;
    Entry* e2;
    PEntry* pentry;

    if (array_size(entries) < 2)
    {
        return NULL;
    }

    pentry = create_default_pentry();
    max_dist = -1;

    for (i = 0; i < array_size(entries) - 1; ++i)
    {
        for (j = i + 1; j < array_size(entries); ++j)
        {
            e1 = array_get(entries, i);
            e2 = array_get(entries, j);

            curr_dist = distance(e1, e2);

            if(curr_dist > max_dist)
            {
                pentry->e1 = e1;
                pentry->e2 = e2;
                max_dist = curr_dist;
            }
        }
    }

    return pentry;
}

PEntry* find_closest_entry_pair(Array* entries, double (*distance)(struct entry*, struct entry*))
{
    int i, j;
    double min_dist;
    double curr_dist;
    Entry* e1;
    Entry* e2;
    PEntry* pentry;

    if (array_size(entries) < 2)
    {
        return NULL;
    }

    pentry = create_default_pentry();
    min_dist = DBL_MAX;

    for (i = 0; i < array_size(entries) - 1; ++i)
    {
        for (j = i + 1; j < array_size(entries); ++j)
        {
            e1 = array_get(entries, i);
            e2 = array_get(entries, j);

            curr_dist = distance(e1, e2);

            if(curr_dist < min_dist)
            {
                pentry->e1 = e1;
                pentry->e2 = e2;
                min_dist = curr_dist;
            }
        }
    }

    return pentry;
}

int count_children_nodes(Node* node)
{
    int i;
    int n;
    Entry* curr_entry;

    n = 0;

    for (i = 0; i < array_size(node->entries); ++i)
    {
        curr_entry = array_get(node->entries, i);

        if (curr_entry->child != NULL)
        {
            n++;
            n += count_children_nodes(curr_entry->child);
        }
    }

    return n;
}

int count_entries_in_children_nodes(Node* node)
{
    int i;
    int n;
    Entry* curr_entry;

    n = 0;

    for (i = 0; i < array_size(node->entries); ++i)
    {
        curr_entry = array_get(node->entries, i);

        if (curr_entry->child != NULL)
        {
            n += array_size(curr_entry->child->entries);
            n += count_children_nodes(curr_entry->child);
        }
    }

    return n;
}

int find_closest_subcluster(Node* node, Entry* entry)
{
    Entry* closest_entry = find_closest_entry(node, entry);

    if(closest_entry->child == NULL)
    {
        return closest_entry->subcluster_id;
    }

    return find_closest_subcluster(closest_entry->child, entry);
}

void redistribute_entries(Node* node, Array* old_entries, PEntry *far_entries, Entry* new_entry_1, Entry* new_entry_2)
{
    int i;
    double dist_1;
    double dist_2;
    Entry* curr_entry;

    for (i = 0; i < array_size(old_entries); ++i)
    {
        curr_entry = array_get(old_entries, i);
        dist_1 = node->distance(far_entries->e1, curr_entry);
        dist_2 = node->distance(far_entries->e2, curr_entry);

        if (dist_1 <= dist_2)
        {
            array_add(new_entry_1->child->entries, curr_entry);
            update_entry(new_entry_1, curr_entry);
        }
        else
        {
            array_add(new_entry_2->child->entries, curr_entry);
            update_entry(new_entry_2, curr_entry);
        }
    }
}

PEntry* split_entry(Node* node, Entry* closest_entry)
{
    // IF there was a child, but we could not insert the new entry without problems THAN
    // split the child of closest entry

    Array* old_entries;
    PEntry* farthest_pair;
    PEntry* new_pair;
    Node* old_node;
    Node* new_node_1;
    Node* new_node_2;
    Node* prev;
    Node* next;
    Entry* new_entry_1;
    Entry* new_entry_2;

    old_node = closest_entry->child;
    old_entries = closest_entry->child->entries;
    farthest_pair = find_farthest_entry_pair(old_entries, node->distance);

    new_entry_1 = create_default_entry();
    new_node_1 = create_node(node->branching_factor, node->threshold, node->distance, old_node->is_leaf, node->apply_merging_refinement);
    new_entry_1->child = new_node_1;

    new_entry_2 = create_default_entry();
    new_node_2 = create_node(node->branching_factor, node->threshold, node->distance, old_node->is_leaf, node->apply_merging_refinement);
    new_entry_2->child = new_node_2;


    if(old_node->is_leaf)
    { // we do this to preserve the pointers in the leafList

        prev = old_node->prev_leaf;
        next = old_node->next_leaf;

        if(prev != NULL)
        {
            prev->next_leaf = new_node_1;
        }

        if(next != NULL)
        {
            next->prev_leaf = new_node_2;
        }

        new_node_1->prev_leaf = prev;
        new_node_1->next_leaf = new_node_2;
        new_node_2->prev_leaf = new_node_1;
        new_node_2->next_leaf = next;
    }


    redistribute_entries(node, old_entries, farthest_pair, new_entry_1, new_entry_2);
    // redistributes the entries in n between newEntry1 and newEntry2
    // according to the distance to p.e1 and p.e2

    array_remove(node->entries, closest_entry); // this will be substitute by two new entries

    free_entry(closest_entry);

    array_add(node->entries, new_entry_1);
    array_add(node->entries, new_entry_2);

    new_pair = create_pentry(new_entry_1, new_entry_2);

    return new_pair;
}

bool insert_entry(Node* node, Entry* entry) {

    Entry* closest_entry;
    bool dont_split;
    PEntry* split_pair;

    if(array_size(node->entries) == 0)
    {
        array_add(node->entries, entry);
        return true;
    }

    closest_entry = find_closest_entry(node, entry);
    dont_split = false;

    if(closest_entry->child != NULL)
    {
        dont_split = insert_entry(closest_entry->child, entry);

        if(dont_split == true)
        {
            update_entry(closest_entry, entry); // this updates the CF to reflect the additional entry
            return true;
        }
        else
        {
            // if the node below /closest/ didn't have enough room to host the new entry
            // we need to split it
            split_pair = split_entry(node, closest_entry);

            // after adding the new entries derived from splitting /closest/ to this node,
            // if we have more than maxEntries we return false,
            // so that the parent node will be split as well to redistribute the "load"
            if(array_size(node->entries) > node->branching_factor)
            {
                return false;
            }
            else
            { // splitting stops at this node
                if(node->apply_merging_refinement)
                { // performs step 4 of insert process (see BIRCH paper, Section 4.3)
                    merging_refinement(split_pair);
                }
                return true;
            }
        }
    }
    else if(is_within_threshold(closest_entry, entry, node->threshold, node->distance))
    {
        // if  dist(closest,e) <= T, /e/ will be "absorbed" by /closest/
        update_entry(closest_entry, entry);
        return true; // no split necessary at the parent level
    }
    else if(array_size(node->entries) < node->branching_factor)
    {
        // if /closest/ does not have children, and dist(closest,e) > T
        // if there is enough room in this node, we simply add e to it
        array_add(node->entries, entry);
        return true; // no split necessary at the parent level
    }
    else
    { // not enough space on this node
        array_add(node->entries, entry); // adds it momentarily to this node
        return false;   // returns false so that the parent entry will be split
    }

}

void merging_refinement(Node* node, PEntry* split_entries)
{

    Node* old_node_1;
    Node* old_node_2;
    Node* new_node_1;
    Node* new_node_2;
    Node* new_node;
    Node* dummy_node;
    PEntry* pentry;
    Array* old_node_1_entries;
    Array* old_node_2_entries;
    Array* node_entries;
    Entry* new_entry_1;
    Entry* new_entry_2;
    Entry* new_entry;

    node_entries = node->entries;
    pentry = find_closest_entry_pair(node_entries, node->distance);

    if (pentry == NULL)
    { // not possible to find a valid pair
        return;
    }

    if (pentry_cmp(pentry, split_entries))
    {
        return; // if the closet pair is the one that was just split, we terminate
    }

    old_node_1 = pentry->e1->child;
    old_node_2 = pentry->e2->child;

    old_node_1_entries = old_node_1->entries;
    old_node_2_entries = old_node_2->entries;

    if(old_node_1->is_leaf != old_node_2->is_leaf) { // just to make sure everything is going ok
        printf("ERROR: birch.c/merging_refinement(): \"Nodes at the same level must have same leaf status\"\n");
        exit(1);
    }

    if((array_size(old_node_1_entries) + array_size(old_node_2_entries)) > node->branching_factor) {
        // the two nodes cannot be merged into one (they will not fit)
        // in this case we simply redistribute them between p.e1 and p.e2

        new_entry_1 = create_default_entry();
        // note: in the CFNode construction below the last parameter is false
        // because a split cannot happen at the leaf level
        // (the only exception is when the root is first split, but that's treated separately)
        new_node_1 = old_node_1;
        array_clear(new_node_1->entries);
        new_entry_1->child = new_node_1;

        new_entry_2 = create_default_entry();
        new_node_2 = old_node_2;
        array_clear(new_node_2->entries);
        new_entry_2->child = new_node_2;

        /**/redistribute_entries(node, old_node_1_entries, old_node_2_entries, pentry, new_entry_1, new_entry_2);
        /**/replace_closest_pair_with_new_entries(pentry, new_entry_1, new_entry_2);

    }
    else {
        // if the the two closest entries can actually be merged into one single entry

        new_entry = create_default_entry();
        // note: in the CFNode construction below the last parameter is false
        // because a split cannot happen at the leaf level
        // (the only exception is when the root is first split, but that's treated separately)
        new_node = create_node(node->branching_factor, node->threshold, node->distance, node->apply_merging_refinement, old_node_1->is_leaf);
        new_entry->child = new_node;

        /**/redistribute_entries(node, old_node_1_entries, old_node_2_entries, new_entry);

        if (old_node_1->is_leaf && old_node_2->is_leaf)
        { // this is done to maintain proper links in the leafList
            if(old_node_1->prev_leaf != NULL)
            {
                old_node_1->prev_leaf->next_leaf = new_node;
            }
            if(old_node_1->next_leaf != NULL)
            {
                old_node_1->next_leaf->prev_leaf = new_node;
            }
            new_node->prev_leaf = old_node_1->prev_leaf;
            new_node->next_leaf = old_node_1->next_leaf;

            // this is a dummy node that is only used to maintain proper links in the leafList
            // no CFEntry will ever point to this leaf
            dummy_node = create_node(0, 0, NULL, true, false);

            if (old_node_2->prev_leaf != NULL)
            {
                old_node_2->prev_leaf->next_leaf = dummy_node;
            }
            if (old_node_2->next_leaf != NULL)
            {
                old_node_2->next_leaf->prev_leaf = dummy_node;
            }

            dummy_node->prev_leaf = old_node_2->prev_leaf;
            dummy_node->next_leaf = old_node_2->next_leaf;
        }

        /**/replace_closest_pair_with_new_merged_entry(pentry, new_entry);
    }

    // merging refinement is done
}
