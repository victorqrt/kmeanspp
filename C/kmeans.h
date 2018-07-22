#ifndef KMEANS
#define KMEANS
#include "dataset.h"

void freeDataset(dataset* myDataset);
void fillDatasetFromStdin(dataset* data);
void displayDataset(dataset* const data);
char** split(char* const str, const char delimiter);
void generate_dataset(int size);

#endif
