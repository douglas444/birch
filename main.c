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
    if (sample->label != NULL)
    {
        free(sample->label);
    }
    free(sample);
}

void print_string_to_file(FILE* f, char* ch)
{
  while(*ch) {
     fputc(*ch, f);
     ch++;
  }
}

char* get_field(char* line, char* delimiters, int num)
{
    char* tok;
    char* tmp = strdup(line);
    for (tok = strtok(tmp, delimiters); tok && *tok; tok = strtok(NULL, delimiters))
    {
        if (!((--num) + 1))
        {
            char* field = strdup(tok);
            free(tmp);
            return field;
        }
    }
    free(tmp);
    return NULL;
}

Array* read_dataset_from_file(char* file_path, int dim, bool last_column_is_label, char* delimiters)
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
        Sample* sample = sample_new(dim);

        for (int i = 0; i < sample->dim; ++i)
        {
            char* field = get_field(line, delimiters, i);
            sscanf(field, "%lf", &sample->x[i]);
            free(field);
        }

        if (last_column_is_label == true)
        {
            sample->label = get_field(line, delimiters, dim); 
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

int main(int argc, char *argv[])
{
    if (argc < 8)
    {
        printf("Arguments missing!\n");
        printf("The following parameters must be informed in the same order and separated by whitespace:\n");
        printf("1 - branching_factor: integer value for the respective birch parameter\n");
        printf("2 - threshold: float value for the respective birch parameter\n");
        printf("3 - apply_merging_refinement: 1 for yes, 0 for no\n");
        printf("4 - file_path: path to the dataset file\n");
        printf("5 - number_of_features: number of features in the dataset\n");
        printf("6 - last_column_is_label: 1 if the last column of the dataset contains the labels, 0 if not\n");
        printf("7 - delimiters: string containing the dataset delimiters\n");
        printf("The line bellow is an example of a valid command line for running this program:\n");
        printf("./main 100 0.8 1 IRIS.csv 4 1 ,\\r\\n\n");
        exit(1);
    }

    int branching_factor = atoi(argv[1]);
    double threshold = atof(argv[2]);
    int apply_merging_refinement = atoi(argv[3]);
    char* file_path = argv[4];
    int number_of_features = atoi(argv[5]);
    int last_column_is_label = atoi(argv[6]);
    char* delimiters = argv[7];

    Array* samples = read_dataset_from_file(file_path, number_of_features, last_column_is_label == 1, delimiters);

    Tree* tree;
    Entry* entry;

    tree = create_tree(branching_factor, threshold, distance, apply_merging_refinement == 1);

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
