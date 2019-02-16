#ifndef KMEANS
#define KMEANS
#include <stdbool.h>

// Data structures

struct cluster;

typedef struct point {
    double x;
    double y;
    struct cluster* cstr;
} point;

typedef struct point_llist {
    point* p;
    struct point_llist* next;
} point_llist;

typedef struct dataset {
    int size;
    point* points;
} dataset;

typedef struct cluster {
    int size;
    point_llist* points;
    point center;
} cluster;

// Data management

void generate_dataset(const int size);
void free_dataset(dataset* myDataset);
void free_cluster(cluster* c);
void add_point_to_cluster(point* p, cluster* c);
void del_point_from_cluster_llist(point_llist* list, point* p);
cluster* initialize_clusters(dataset* const data, int c_nb);
dataset* initialize_dataset(const int size);
dataset* dataset_from_csv(const char* filename);

// Useful computations

int cluster_size(cluster c);
double distance(const point p1, const point p2);
bool update_clusters(dataset const data, cluster* clusters, int c_nb);
void cluster_update_loop(dataset d, cluster* c, int c_nb);
point barycenter_from_list(point* const point_list, const int size);
point cluster_barycenter(cluster c);

// Misc

char** split(char* const str, const char delimiter);
void export_csv();

#endif
