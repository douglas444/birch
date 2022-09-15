#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdbool.h>
#include "array.h"

double* read_instance(char* line, int dimensionality, char* delimiters);
int calculate_dimensionality(char* line, char* delimiters, bool last_column_is_label);

#endif

