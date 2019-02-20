#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "kmeans.h"

dataset* initialize_dataset(const int size) {
    dataset* myDataset = (dataset*) malloc(sizeof(dataset));
    myDataset->size = size;
    myDataset->points = (point*) malloc(myDataset->size * sizeof(point));
    return myDataset;
}

#define _getline_or_die(line, fd) if(!fgets(line, 256, fd)) {printf("[!] File error\n"); exit(EXIT_FAILURE);}

dataset* dataset_from_csv(const char* name) {
    printf("[ ] Loading dataset...\n");

    FILE* f = fopen(name, "r");
    int lines = 0;

    if(!f)
        return NULL;

    for(char c = fgetc(f); c != EOF; c = fgetc(f))
        if(c == '\n') lines++;

    // This assumes the csv contains no empty lines nor header
    dataset* myDataset = initialize_dataset(lines);

    rewind(f);
    for(int i = 0; i<lines; i++) {
        char line[256];

        _getline_or_die(line, f)

        char** splitted_line = split(line, ',');
        myDataset->points[i].x = atof(splitted_line[0]);
        myDataset->points[i].y = atof(splitted_line[1]);
        myDataset->points[i].cstr = NULL;
        free(splitted_line);
    }

    fclose(f);
    return myDataset;
}

void free_dataset(dataset* myDataset) {
    free(myDataset->points);
    free(myDataset);
}

char** split(char* const str, const char delimiter) {
    // We know we're splitting 3 fields at most
    #define len 3
    char** splitted = malloc(len * sizeof(char*));
    char* _str = strdup(str);

    for(int i = 0; i<len; i++)
        splitted[i] = strsep(&_str, &delimiter);

    free(_str);
    return splitted;
}

void generate_dataset(const int size) {
    printf("[ ] Generating %d 2D points into dataset.csv...\n", size);

    FILE* f = fopen("dataset.csv", "w");

    if(!f)
        printf("[!] File error\n");
    else {
        for(int i=0; i<size; i++)
            fprintf(f, "%d,%d\n", rand() % 1000, rand() % 1000);

        fclose(f);
        printf("[+] Done.\n");
    }
}

void export_csv(dataset* const data) {
    FILE* f = fopen("out.csv", "w+");

    if(!f)
        printf("[!] File error\n");
    else {
        for(int i=0; i < data->size; i++)
            fprintf(f, "%d,%d,%d\n", (int) data->points[i].x, (int) data->points[i].y, data->points[i].cstr->id);

        fclose(f);
        printf("[+] Done.\n");
    }
}

void export_svg() {
    printf("[ ] Exporting points from out.csv to out.svg...\n");

    FILE* f = fopen("out.csv", "r");

    if(!f)
        printf("[!] File error\n");
    else {
        int lines = 0;
        for(char c = fgetc(f); c != EOF; c = fgetc(f))
            if(c == '\n') lines++;
        rewind(f);

        point myPoints[lines];
        int clusters[lines];

        for(int i=0; i<lines; i++) {
            char line[256];

            _getline_or_die(line, f)

            char** splitted_line = split(line, ',');
            myPoints[i].x = atof(splitted_line[0]);
            myPoints[i].y = atof(splitted_line[1]);
            clusters[i] = atoi(splitted_line[2]);
            free(splitted_line);
        }

        fclose(f);
        f = fopen("out.svg", "w+");

        if(!f)
            printf("[!] File error\n");
        else {
            char* colors[] = {"black", "blue", "red", "green", "yellow", "gray", "gold", "tan", "olivedrab", "aqua"};
            fprintf(f, "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='1000' height='1000'>\n");

            for(int i=0; i<lines; i++)
                fprintf(f, "    <circle cx='%f' cy='%f' r='5' fill='%s' />\n", myPoints[i].x, myPoints[i].y, colors[clusters[i] % 10]);

            fprintf(f, "</svg>\n");
            fclose(f);
            printf("[+] Done.\n");
        }
    }
}

double distance(const point p1, const point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

point barycenter_from_list(point* const point_list, const int size) {
    double sx, sy;
    sx = sy = 0;

    for(int i=0; i<size; i++) {
        sx += point_list[i].x;
        sy += point_list[i].y;
    }

    point p = { sx/size, sy/size, NULL};
    return p;
}

void add_point_to_cluster(point* p, cluster* c) {
    point_llist* current_p = c->points;
    while(current_p && current_p->next)
        current_p = current_p->next;

    if(current_p) {
        current_p->next = malloc(sizeof(point_llist));
        current_p->next->p = p;
        current_p->next->next = NULL;
    }
    else {
        current_p = malloc(sizeof(point_llist));
        current_p->p = p;
        current_p->next = NULL;
        c->points = current_p;
    }

    c->size++;
    p->cstr = c;
}

void del_point_from_cluster_llist(point_llist* list, point* p) {
    // Deletes a point from a linked list, by reference
    for(point_llist** ref = &list; *ref; ref = &(*ref)->next) {
        if((*ref)->p == p)
            *ref = (*ref)->next;
    }
}

void free_cluster(cluster* c) {

    void free_pllist(point_llist* p) {
        if(p) {
            if(p->next)
                free_pllist(p->next);
            free(p);
        }
    }

    free_pllist(c->points);
    free(c);
}

cluster* initialize_clusters(dataset* const data, int c_nb) {

    printf("[ ] Choosing initial cluster centers...\n");

    if(data->size < c_nb) {
        printf("[!] Error: we are looking for more clusters than there are points in the dataset\n");
        return NULL;
    }

    int rand_idx[c_nb];

    for(int i=0; i<c_nb; i++) {
        rand_idx[i] = rand() % data->size;

        for(int j=0; j<i; j++) { // We want unique indices
            if(rand_idx[i] == rand_idx[j]) {
                rand_idx[i] = rand() % data->size;
                j = 0;
            }
        }
    }

    cluster* cluster_list = malloc(c_nb * sizeof(cluster));
/*
    point* current_center = data->points + rand_idx[0];
    for(int c=0; c<c_nb; c++) {
        int nb_candidates = data->size - c_nb;
        int current_candidate_index = 0;
        point* candidate_points[nb_candidates];

        for(int i=0; i<data->size; i++) {
            bool do_add = true;

            for(int j=0; j<c_nb; j++) {
                for(int k=0; k<current_candidate_index; k++)
                    if(data->points + rand_idx[j] == candidate_points[k]) do_add = false;
            }

            if(do_add) {
                candidate_points[current_candidate_index] = data->points + i;
                current_candidate_index++;
            }
        }

        double weight_dividor = 0;

        for(int i=0; i<nb_candidates; i++)
            weight_dividor += pow(distance(*candidate_points[i], *current_center), 2);

        double max_weight = 0;
        point* best_candidate = NULL;

        for(int i=0; i<nb_candidates; i++) {
            double weight = pow(distance(*candidate_points[i], *current_center), 2) / weight_dividor;

            if(weight > max_weight) {
                max_weight = weight;
                best_candidate = candidate_points[i];
            }
        }

        printf("%f, %f - candidate for cluster %d\n", best_candidate->x, best_candidate->y, c);
        current_center = best_candidate;
        cluster_list[c] = (cluster) {0, NULL, *current_center};
    }
*/

    // Random initialization for now. Above (unfinished) code aims at better choosing initial centers,
    // putting the ++ in kmeans.

    for(int i=0; i<c_nb; i++) {
        cluster_list[i] = (cluster) {0, i, NULL, data->points[rand_idx[i]]};
        cluster_list[i].center.cstr = cluster_list + i;
    }

    for(int i=0; i<data->size; i++) {
        cluster* best_cluster = cluster_list;
        for(int j=0; j<c_nb; j++) {
            if(distance(cluster_list[j].center, data->points[i]) < distance(best_cluster->center, data->points[i]))
                best_cluster = cluster_list + j;
        }

        add_point_to_cluster(data->points + i, best_cluster);
    }

    return cluster_list;
}

int cluster_size(cluster c) {
    int s = 0;
    for(point_llist* p = c.points; p; p = p->next, s++);
    return s;
}

point cluster_barycenter(cluster c) {
    point plist[c.size];

    int i = 0;
    for(point_llist* p = c.points; p; p = p->next, i++)
        plist[i] = *(p->p);

    return barycenter_from_list(plist, c.size);
}

bool update_clusters(dataset const data, cluster* clusters, int c_nb) {
    bool modified = false;

    for(int i=0; i<c_nb; i++) {

        clusters[i].center = cluster_barycenter(clusters[i]);

        for(int j=0; j<data.size; j++) {

            if(distance(clusters[i].center, data.points[j]) < distance(data.points[j].cstr->center, data.points[j])) {
                // del_point_from_cluster_llist(data.points[j].cstr->points, data.points + j);
                add_point_to_cluster(data.points + j, clusters + i);
                modified = true;
            }
        }

        clusters[i].size = cluster_size(clusters[i]);
    }

    return modified;
}
