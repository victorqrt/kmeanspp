#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dataset.h"
#include "kmeans.h"

void usage() {
    printf("Usage:\n    kmeanspp generate_dataset <dataset_size>\n    kmeanspp clusterize <dataset> <number_of_clusters>\n    kmeanspp export\n");
}

int main(int argc, char** argv) {
    if(argc < 2)
        usage();
    else {
        if(strcmp(argv[1], "generate_dataset") == 0 && argc == 3) {
            srand(time(NULL));
            generate_dataset(atoi(argv[2]));
        }
        else if(strcmp(argv[1], "clusterize") == 0 && argc == 4) {
            srand(time(NULL));
            dataset* myDataset = dataset_from_csv(argv[2]);
            cluster* c = initialize_clusters(myDataset, atoi(argv[3]));
            if(c) {
                //clusterize
                free_cluster(c);
            }
            free_dataset(myDataset);
        }
        else if(strcmp(argv[1], "export") == 0)
            export_csv();
        else
            usage();
    }

    return 0;
}
