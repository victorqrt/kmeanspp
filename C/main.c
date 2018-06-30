#include <stdio.h>
#include <stdlib.h>
#include "dataset.h"
#include "kmeans.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage : kmeanspp dataset_file\n");
        return 0;
    }

    dataset* mydata = initializeDataset(atoi(argv[1]));
    fillDatasetFromStdin(mydata);
    displayDataset(mydata);
    freeDataset(mydata);
    return 0;
}
