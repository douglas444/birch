#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "smem.h"
#include "sample.h"

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

int count_tokens(char* line, char* delimiters)
{
    char *p;
    int count = 0;
    char* tmp = strdup(line);

    p = strtok(tmp, delimiters);

    while (p != NULL)
    {
        count++;
        p = strtok(NULL, delimiters);
    }

    free(tmp);

    return count;
}

Array* read_dataset_from_file(char* file_path, bool last_column_is_label, char* delimiters)
{
    Array* samples = array_new(1);
    FILE* stream = fopen(file_path, "r");
    if (stream == NULL)
    {
        printf("ERROR: sample.c/read_dataset_from_file(): \"Error opening file\"\n");
        exit(1);
    }

    char line[1024];
    while (fgets(line, 1024, stream))
    {
        int columns = count_tokens(line, delimiters);

        int dim = (last_column_is_label == true) ? columns - 1 : columns;
        Sample* sample = sample_new(dim);

        for (int i = 0; i < sample->dim; ++i)
        {
            char* field = get_field(line, delimiters, i);
            sscanf(field, "%lf", &sample->x[i]);
            free(field);
        }

        if (last_column_is_label == true)
        {
            sample->label = get_field(line, delimiters, columns - 1);
        }

        array_add(samples, sample);
    }

    fclose(stream);

    return samples;
}
