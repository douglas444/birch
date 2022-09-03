#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "smem.h"
#include "birch.h"

struct sample
{
    int dim;
    double* x;
    char* label;

};
typedef struct sample Sample;

Sample* sample_new(int dim)
{
    Sample *sample = (Sample*) smalloc(sizeof(Sample));
    sample->x = smalloc(sizeof(double*) * dim);
    sample->label = NULL;
    sample->dim = dim;
    return sample;
}

void sample_free(Sample *sample)
{
    free(sample->x);
    free(sample);
}

void print_string_to_file(FILE* f, char *ch)
{
  while(*ch) {
     fputc(*ch, f);
     ch++;
  }
}

char* get_field(char* line, int num)
{
    char* tok;
    char* tmp = strdup(line);
    for (tok = strtok(tmp, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!((--num) + 1))
        {
            return tok;
        }
    }
    free(tmp);
    return NULL;
}

Array* read_dataset_from_file(char* file_path, int dim, bool is_label_available)
{
    Array* samples = array_new(1);
    FILE* stream = fopen(file_path, "r");
    if (stream == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        char *pos;
        if ((pos=strchr(line, '\r')) != NULL)
        {
            *pos = '\0';
        }

        Sample* sample = sample_new(dim);

        for (int i = 0; i < sample->dim; ++i)
        {
            char* field = get_field(line, i);
            sscanf(field, "%lf", &sample->x[i]);
        }

        if (is_label_available == true)
        {
            sample->label = get_field(line, dim);
        }

        array_add(samples, sample);
    }

    fclose(stream);

    return samples;
}

double distance(Entry* e1, Entry* e2)
{
    double dist = 0;
    for (int i = 0; i < e1->dim; ++i)
    {
        double diff = (e1->ls[i] / e1->n) - (e2->ls[i] / e2->n);
        dist += diff * diff;
    }
    return sqrt(dist);
}

int main()
{
    int number_of_features = 4;
    Array* samples = read_dataset_from_file("IRIS.csv", number_of_features, true);

    Tree* tree;
    Entry* entry;

    tree = create_tree(5, 0.5, distance, false);

    for (int i = 0; i < array_size(samples); ++i)
    {
        Sample* sample = array_get(samples, i);
        entry = create_entry(sample->x, number_of_features, i);
        insert_entry_in_tree(tree, entry);
    }

    FILE *f = fopen("output.csv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int* cluster_id_by_entry_index = get_cluster_id_by_entry_index(tree);
    for (int i = 0; i < array_size(samples); ++i)
    {
        Sample* sample = (Sample*) array_get(samples, i);
        for(int j = 0; j < sample->dim; j++)
        {
            fprintf(f, "%g,", sample->x[j]);
        }
        if (sample->label != NULL)
        {
            print_string_to_file(f, sample->label);
        }
        fprintf(f, ",cluster_%d\n", cluster_id_by_entry_index[i]);
    }

    fclose(f);


    free(cluster_id_by_entry_index);
    free_tree(tree);
    for (int i = 0; i < array_size(samples); ++i) {
        sample_free(array_get(samples, i));
    }

    array_free(samples);

    return 0;
}
