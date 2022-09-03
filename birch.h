#include <stdbool.h>
#include "array.h"

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
    int insert_count;
};


typedef struct entry Entry;
typedef struct node Node;
typedef struct pentry PEntry;
typedef struct tree Tree;

void free_tree(Tree* tree);
Tree* create_tree(int branching_factor, double threshold, double (*distance)(struct entry*, struct entry*), bool apply_merging_refinement);
void insert_entry_in_tree(Tree* tree, Entry* entry);
Entry* create_entry(double* x, int dim, int index);
Array* get_subclusters(Tree* tree);
int* get_cluster_id_by_entry_index(Tree* tree);
void free_entry(Entry *entry);
