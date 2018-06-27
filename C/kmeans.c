#include <stdio.h>
#include <stdlib.h>
#include "kmeans.h"
#include "dataset.h"

dataset* initializeDataset(int size)
{
    dataset* myDataset = (dataset*) malloc(sizeof(dataset));
    myDataset->size = size;
    myDataset->points = (point*) malloc(myDataset->size * sizeof(point));
    return myDataset;
}

void freeDataset(dataset* myDataset)
{
    free(myDataset->points);
    free(myDataset);
}

void fillDatasetFromStdin(dataset* data)
{
    printf("Enter coordinates for the %d points of the dataset:\n", data->size);
    for(int i=0; i<data->size; i++)
    {
        printf("\tEnter x coord of point %d: ", i+1);
        scanf("%d", &data->points[i].x);
        printf("\tEnter y coord of point %d: ", i+1);
        scanf("%d", &data->points[i].y);
    }
}

void displayDataset(dataset* data)
{
    printf("Dataset:\n");
    for(int i=0; i<data->size; i++)
            printf("\tPoint %d of %d: x=%d, y=%d\n", i+1, data->size, data->points[i].x, data->points[i].y);
}
