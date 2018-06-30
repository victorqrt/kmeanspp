#ifndef DATASET
#define DATASET

typedef struct point
{
    int x;
    int y;
}point;

typedef struct dataset
{
    int size;
    point* points;
}dataset;

typedef struct cluster
{
    int size;
    point* points;
}cluster;

typedef struct model
{
    dataset data;
    int numberOfClusters;
    cluster* clusters;
}model;

dataset* initializeDataset(const int size);
dataset* datasetFromCSV(const char* filename);

#endif
