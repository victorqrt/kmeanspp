#ifndef KMEANS
#define KMEANS
#include "dataset.h"

void freeDataset(dataset* myDataset);
void displayDataset(dataset* const data);
char** split(char* const str, const char delimiter);
void generate_dataset(const int size);
void export_csv();
double distance(const point p1, const point p2);
point barycenterFromList(point* const point_list, const int size);
cluster* initialize_clusters(dataset* const data, int c_nb);

#endif
