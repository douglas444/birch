#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "birch.h"
#include "array.h"
#include "smem.h"

#define INDEXES_INITIAL_SIZE 4

struct node {
    Array *entries;
    int branching_factor;
    double threshold;
    bool is_leaf;
    double (*distance)(struct entry*, struct entry*);
    struct node *next_leaf;
    struct node *prev_lead;
};

struct entry {
    int dim;
    int n;
    double *ls;
    double *ss;
    struct node *child;
    Array *indexes;
    int subcluster_id;
};

typedef struct entry Entry;
typedef struct node Node;

Entry* new_entry_default()
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

Entry* new_entry(double *x, int dim, int index)
{
    int i;

    Entry *entry = new_entry_default();

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

bool is_within_threshold(Entry *e1, Entry *e2, double threshold, double (*distance)(Entry*, Entry*))
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
