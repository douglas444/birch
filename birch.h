typedef struct entry Entry;
typedef struct node Node;

Entry* new_entry_default();
Entry* new_entry(double *x, int dim, int index);
void update_entry(Entry *e1, Entry *e2);
