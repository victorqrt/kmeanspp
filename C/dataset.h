#ifndef DATASET
#define DATASET

struct cluster;

typedef struct point
{
    double x;
    double y;
    struct cluster* cstr;
}point;

typedef struct dataset
{
    int size;
    point* points;
    point center;
}dataset;

typedef struct cluster
{
    int size;
    point* points;
}cluster;

dataset* initializeDataset(const int size);
dataset* datasetFromCSV(const char* filename);

#endif
