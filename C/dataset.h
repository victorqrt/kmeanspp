#ifndef DATASET
#define DATASET

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

dataset* initialize_dataset(const int size);
dataset* dataset_from_csv(const char* filename);
void add_point_to_cluster(point* p, cluster* c);
void free_cluster(cluster* c);

#endif
