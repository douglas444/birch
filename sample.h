#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdbool.h>
#include "array.h"

struct sample
{
    int dim;
    double* x;
    char* label;

};

typedef struct sample Sample;

Sample* sample_new(int dim);
void sample_free(Sample *sample);
Array* read_dataset_from_file(char* file_path, bool last_column_is_label, char* delimiters);

#endif

