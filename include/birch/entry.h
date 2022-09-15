#ifndef ENTRY_H
#define ENTRY_H

#include "../util/array.h"

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
typedef struct entry Entry;

Entry* entry_create_default(int dim);
Entry* entry_create(double* x, int dim, int index);
void entry_free(Entry *entry);
void entry_update(Entry *e1, Entry *e2);
bool entry_is_within_threshold(Entry *e1, Entry *e2, double threshold, double (*distance)(Entry*, Entry*));
bool entry_cmp(Entry *e1, Entry *e2);
void entry_remove(Array* entries, Entry* entry);

#endif
