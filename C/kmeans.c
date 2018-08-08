#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "kmeans.h"
#include "dataset.h"

dataset* initializeDataset(const int size)
{
    dataset* myDataset = (dataset*) malloc(sizeof(dataset));
    myDataset->size = size;
    myDataset->points = (point*) malloc(myDataset->size * sizeof(point));
    return myDataset;
}

dataset* datasetFromCSV(const char* name)
{
    FILE* f = fopen(name, "r");
    int lines = 0;

    if(!f)
        return NULL;

    for(char c = fgetc(f); c != EOF; c = fgetc(f))
        if(c == '\n') lines++;

    // This assumes the csv contains no empty lines nor header
    dataset* myDataset = initializeDataset(lines);

    rewind(f);
    for(int i = 0; i<lines; i++)
    {
        char line[256];
        fgets(line, 256, f);
        char** splitted_line = split(line, ',');
        myDataset->points[i].x = atof(splitted_line[0]);
        myDataset->points[i].y = atof(splitted_line[1]);
        myDataset->points[i].cstr = NULL;
        free(splitted_line);
    }
    
    fclose(f);
    return myDataset;
}

void freeDataset(dataset* myDataset)
{
    free(myDataset->points);
    free(myDataset);
}

char** split(char* const str, const char delimiter)
{
    // We know we're splitting 3 fields at most
    #define len 3
    char** splitted = malloc(len * sizeof(char*));
    char* _str = strdup(str);
    char* token;

    for(int i = 0; i<len; i++)
        splitted[i] = strsep(&_str, &delimiter);

    free(_str);    
    return splitted;
}

void generate_dataset(const int size)
{
    printf("[ ] Generating %d 2D points into dataset.csv...\n", size);

    FILE* f = fopen("dataset.csv", "w");
    
    if(!f)
        printf("[!] File error\n");
    else
    {
        for(int i=0; i<size; i++)
            fprintf(f, "%d,%d\n", rand() % 1000, rand() % 1000);

        fclose(f);
        printf("[+] Done.\n");
    }
}

void export_csv()
{
    printf("[ ] Exporting points from out.csv to out.svg...\n");
    
    FILE* f = fopen("out.csv", "r");
    
    if(!f)
        printf("[!] File error\n");
    else
    {
        int lines = 0;
        for(char c = fgetc(f); c != EOF; c = fgetc(f))
            if(c == '\n') lines++;
        rewind(f);
       
        point myPoints[lines];
        int clusters[lines];

        for(int i=0; i<lines; i++)
        {
            char line[256];
            fgets(line, 256, f);
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
        else
        {
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

double distance(const point p1, const point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

point barycenterFromList(point* const point_list, const int size)
{
    double sx, sy = 0;
    
    for(int i=0; i<size; i++)
    {
        sx += point_list[i].x;
        sy += point_list[i].y;
    }

    point p = { sx/size, sy/size, NULL};
    return p;
}

cluster* initialize_clusters(dataset* const data, int c_nb)
{
    int rand_idx[c_nb];

    for(int i=0; i<c_nb; i++)
    { 
        rand_idx[i] = rand() % data->size;
        for(int j=0; j<i; j++) // We want unique indices
        {
            if(rand_idx[i] == rand_idx[j])
            {
                rand_idx[i] = rand() % data->size;
                j = 0;
            }
        }
    }
        
    point* current_center = data->points + rand_idx[0];
    cluster* cluster_list = malloc(c_nb * sizeof(cluster));
  
    for(int c=0; c<c_nb; c++)
    {
        int nb_candidates = data->size - c_nb;
        int current_candidate_index = 0;
        point* candidate_points[nb_candidates];
    
        for(int i=0; i<data->size; i++)
        {
            bool do_add = true;
            for(int j=0; j<c_nb; j++)
            {
                for(int k=0; k<current_candidate_index; k++)
                    if(data->points[rand_idx[j]].x == candidate_points[k]->x && data->points[rand_idx[j]].y == candidate_points[k]->y) do_add = false;
            }

            if(do_add)
            {
                candidate_points[current_candidate_index] = data->points + i;
                current_candidate_index++;
            }
        }

        double weight_dividor = 0;
        printf("%d, %d\n", nb_candidates, current_candidate_index);
    
        for(int i=0; i<nb_candidates; i++)
            weight_dividor += pow(distance(*candidate_points[i], *current_center), 2);

        double max_weight = 0;
        point* best_candidate = NULL;

        for(int i=0; i<nb_candidates; i++)
        {
            double weight = pow(distance(*candidate_points[i], *current_center), 2);
            if(weight > max_weight)
            {
                max_weight = weight;
                best_candidate = candidate_points[i];
            }
        }

        current_center = best_candidate;
        cluster_list[c] = (cluster) {0, NULL, *current_center};

    }

    return cluster_list;
}
