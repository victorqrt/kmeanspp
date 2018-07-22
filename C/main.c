#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"
#include "kmeans.h"

void usage()
{
    printf("Usage:\n    kmeanspp generate_dataset <dataset_size>\n    kmeanspp clusterize <dataset> <number_of_clusters>\n    kmeanspp export\n");
}

int main(int argc, char** argv)
{
    if(argc < 2)
        usage();
    else
    {
        if(strcmp(argv[1], "generate_dataset") == 0)
        {
            if(argc != 3)
            {
                usage();
                return 1;
            }
            
            generate_dataset(atoi(argv[2]));
        }
        else if(strcmp(argv[1], "clusterize") == 0)
            printf("clusterize");
        else if(strcmp(argv[1], "export") == 0)
            printf("export");
        else
            usage();
    }

    return 0;
}
