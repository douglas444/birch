#include <stdbool.h>
typedef struct entry Entry;
typedef struct node Node;
typedef struct pentry PEntry;
typedef struct tree Tree;

void free_tree(Tree* tree);
Tree* create_tree(int branching_factor, double threshold, double (*distance)(struct entry*, struct entry*), bool apply_merging_refinement);
bool insert_entry_in_tree(Tree* tree, Entry* entry);
Entry* create_entry(double* x, int dim, int index);
void free_entry(Entry *entry);
