#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define AREAS 10000
#define BUFFER 200

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_sec - t_start->tv_sec) * 1000000000L + 
           (t_stop->tv_nsec - t_start->tv_nsec);
}

typedef struct area {
    char *name;
    char *zip;
    int pop;
} area;

typedef struct codes {
    area *areas;
    int n;
} codes;

codes *read_postcodes(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    if (!postnr) return NULL;

    area *areas = (area*)malloc(sizeof(area)*AREAS);
    if (!areas) {
        free(postnr);
        return NULL;
    } 

    FILE *ftpr = fopen(file, "r");
    if (!ftpr) {
        free(areas);
        free(postnr);
        return NULL;
    }

    int k = 0;

    char *lineptr = malloc(sizeof(char)*BUFFER);
    size_t n = BUFFER;

    while((k < AREAS) && (getline(&lineptr, &n, ftpr) > 0)) {
        char *copy = (char*)malloc(sizeof(char)*n);
        strcpy(copy, lineptr);

        area a;

        char *token = strtok(copy, ",");
        a.zip = malloc(strlen(token) + 1);
        strcpy(a.zip, token);

        token = strtok(NULL, ",");
        a.name = malloc(strlen(token) + 1);
        strcpy(a.name, token);

        token = strtok(NULL, ",");
        a.pop = atoi(token);

        areas[k++] = a;
        free(copy);
    }

    fclose(ftpr);
    free(lineptr);

    postnr->areas = areas;
    postnr->n = k;

    return postnr;
}

void free_codes(codes*postnr) {
    if (!postnr) return;
    for (int i = 0; i < postnr->n; i++) {
        free(postnr->areas[i].zip);
        free(postnr->areas[i].name);
    }
    free(postnr->areas);
    free(postnr);
}

bool binary_search(codes *postnr, const char *target_zip) {
    int first = 0;
    int last = postnr->n - 1;
    
    while (first <= last) {
        int mid = (first + last)/2;

        //compare target zip with zip code at mid
        int cmp = strcmp(target_zip, postnr->areas[mid].zip);

        if (cmp == 0) {
            //target equals mid's zip
            return true;
        } else if (cmp < 0) {
            //Target_zip is less than mid's zip
            //Search in the left half
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }

    return false; //not found
}

long benchmark_search(codes *postnr, const char *zip, int iterations) {
    long min_time = LONG_MAX;

    for (int i = 0; i < iterations; i++) {
        struct timespec start, stop;
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        bool result = binary_search(postnr, zip);
        clock_gettime(CLOCK_MONOTONIC, &stop);

        if (result) {
            long elapsed = nano_seconds(&start, &stop);
            if (elapsed < min_time) {
                min_time = elapsed;
            }
        }
    }

    return min_time;
}
  

int main() {

    struct timespec start, stop;

    codes *stockholm = read_postcodes("postnummer.csv");
    if(!stockholm) {
        printf("Failed to read postcodes\n");
        return 1;
    }

    

    printf("\n--- Benchmarking ---\n");
    int iterations = 10000;
    
    const char *zip1 = "111 15";
    long time1 = benchmark_search(stockholm, zip1, iterations);
    printf("%s: %ld ns (min over %d iterations)\n", zip1, time1, iterations);

    const char *zip2 = "984 99";
    long time2 = benchmark_search(stockholm, zip2, iterations);
    printf("%s: %ld ns (min over %d iterations)\n", zip2, time2, iterations);

    free_codes(stockholm);
    return 0;
}
