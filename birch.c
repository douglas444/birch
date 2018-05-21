#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "array.h"
#include "smem.h"

struct node {
    Array *entries;
    int branching_factor;
    double threshold;
    bool is_leaf;
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


