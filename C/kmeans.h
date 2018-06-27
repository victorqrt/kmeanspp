#ifndef KMEANS
#define KMEANS
#include "dataset.h"

dataset* initializeDataset(int size);
void freeDataset(dataset* myDataset);
void fillDatasetFromStdin(dataset* data);
void displayDataset(dataset* data);

#endif
