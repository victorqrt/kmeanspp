#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        myDataset->points[i].x = atoi(splitted_line[0]);
        myDataset->points[i].y = atoi(splitted_line[1]);
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

void generate_dataset(int size)
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
            myPoints[i].x = atoi(splitted_line[0]);
            myPoints[i].y = atoi(splitted_line[1]);
            clusters[i] = atoi(splitted_line[2]);
        }

        fclose(f);
        f = fopen("out.svg", "w+");
        
        if(!f)
            printf("[!] File error\n");
        else
        {
            #define nb_colors 10
            char* colors[] = {"black", "blue", "red", "green", "yellow", "gray", "gold", "tan", "olivedrab", "aqua"};
            fprintf(f, "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='1000' height='1000'>\n");

            for(int i=0; i<lines; i++)
                fprintf(f, "    <circle cx='%d' cy='%d' r='5' fill='%s' />\n", myPoints[i].x, myPoints[i].y, colors[clusters[i] % nb_colors]);

            fprintf(f, "</svg>\n");
            fclose(f);
            printf("[+] Done.\n");
        }
    }
}
