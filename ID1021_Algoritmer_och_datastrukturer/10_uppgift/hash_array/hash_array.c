#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define AREAS 100000
#define BUFFER 200

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_sec - t_start->tv_sec) * 1000000000L +
            (t_stop->tv_nsec - t_start->tv_nsec);
}

typedef struct area {
    char *name;
    int zip;
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

    while ((k < AREAS) && (getline(&lineptr, &n, ftpr) > 0)) {
        char *copy = (char*)malloc(sizeof(char)*n);
        strcpy(copy, lineptr);

        area a;

        char *zip = strtok(copy, ",");
        a.zip = atoi(zip)*100 + atoi(zip+3);

        char *token = strtok(NULL, ",");
        a.name = malloc(strlen(token) +1);
        strcpy(a.name, token);

        token = strtok(NULL, ",");
        a.pop = atoi(token);

        areas[a.zip] = a;
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
        free(postnr->areas[i].name);
    }

    free(postnr->areas);
    free(postnr);
}

bool look_up(codes *postnr, int zip) {
    return postnr->areas[zip].zip == zip;
}

long benchmark_search(codes *postnr, int zip, int iterations) {
    long min_time = LONG_MAX;

    for (int i = 0; i < iterations; i++) {
        struct timespec start, stop;
        clock_gettime(CLOCK_MONOTONIC, &start);
        look_up(postnr, zip);
        clock_gettime(CLOCK_MONOTONIC, &stop);

        long elapsed = nano_seconds(&start, &stop);
        if (elapsed < min_time) {
            min_time = elapsed;
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
    
    int zip1 = 11115;
    long time1 = benchmark_search(stockholm, zip1, iterations);
    printf("%d: %ld ns (min over %d iterations)\n", zip1, time1, iterations);

    int zip2 = 98499;
    long time2 = benchmark_search(stockholm, zip2, iterations);
    printf("%d: %ld ns (min over %d iterations)\n", zip2, time2, iterations);

    free_codes(stockholm);
    return 0;
}