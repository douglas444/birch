#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "smem.h"
#include "birch.h"
#include "sample.h"

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

void print_string_to_file(FILE* f, char* ch)
{
    while(*ch)
    {
        fputc(*ch, f);
        ch++;
    }
}

void print_output(Array* samples, Tree* tree, char* column_delimiter)
{
    FILE *f = fopen("cbirch_output.csv", "w");
    if (f == NULL)
    {
        printf("ERROR: main.c/print_output(): \"Error opening file\"\n");
        exit(1);
    }

    int* cluster_id_by_entry_index = get_cluster_id_by_entry_index(tree);
    for (int i = 0; i < array_size(samples); ++i)
    {
        Sample* sample = (Sample*) array_get(samples, i);

        for(int j = 0; j < sample->dim; j++)
        {
            fprintf(f, "%g", sample->x[j]);
            fprintf(f, column_delimiter);
        }
        if (sample->label != NULL)
        {
            print_string_to_file(f, sample->label);
            fprintf(f, column_delimiter);
        }
        fprintf(f, "cluster_%d\n", cluster_id_by_entry_index[i]);
    }

    fclose(f);

    free(cluster_id_by_entry_index);
}

void print_arguments_message()
{
    printf("\n>>> Invalid numbers of arguments! <<<\n");
    printf("\nThe following parameters must be informed in this order and separated by whitespace:\n");
    printf("\n[1] - Branching Factor: An integer value.\n");
    printf("\n[2] - Threshold: A float value.\n");
    printf("\n[3] - Apply Merging Refinement: 1 for yes, 0 for no.\n");
    printf("\n[4] - Dataset File Path: String.\n");
    printf("\n[5] - Dataset Delimiters: String.\n");
    printf("\n[6] - Ignore Last Column of the Dataset: 1 for yes, 0 for no.\n");
    printf("\n\nThe line bellow is an example of a valid command line for running this program:\n");
    printf("./main 100 0.8 1 IRIS.csv , 1\n\n");
}

int main(int argc, char* argv[])
{
    if (argc < 7)
    {
        print_arguments_message();
        exit(1);
    }

    printf("Execution started...\n");

    int branching_factor = atoi(argv[1]);
    double threshold = atof(argv[2]);
    int apply_merging_refinement = atoi(argv[3]);
    char* file_path = argv[4];
    char* column_delimiter = argv[5];
    int last_column_is_label = atoi(argv[6]);

    char* delimiters = smalloc(sizeof(char*) * (strlen(column_delimiter) + 2 + 1));
    strcat(delimiters, column_delimiter);
    strcat(delimiters, "\r\n");

    Array* samples = read_dataset_from_file(file_path, last_column_is_label == 1, delimiters);

    free(delimiters);

    Tree* tree;
    Entry* entry;

    tree = create_tree(branching_factor, threshold, distance, apply_merging_refinement == 1);
    int samples_size = array_size(samples);

    for (int i = 0; i < samples_size; ++i)
    {
        Sample* sample = array_get(samples, i);
        entry = entry_create(sample->x, sample->dim, i);
        insert_entry_in_tree(tree, entry);
    }

    print_output(samples, tree, column_delimiter);

    free_tree(tree);
    for (int i = 0; i < array_size(samples); ++i)
    {
        sample_free(array_get(samples, i));
    }
    array_free(samples);

    printf("Execution finished.\n");

    return 0;
}


