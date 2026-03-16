#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define AREAS 20000
#define BUFFER 200
#define MOD 10000

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
    int *zips;
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

    int *zips = (int*)malloc(sizeof(int) * AREAS);
    if (!zips) {
        free(areas);
        free(postnr);
        return NULL;
    }
    postnr->zips = zips;

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
        postnr->zips[k] = a.zip;

        char *token = strtok(NULL, ",");
        a.name = malloc(strlen(token) + 1);
        strcpy(a.name, token);

        token = strtok(NULL, ",");
        a.pop = atoi(token);

        int index = a.zip % MOD;
        while (areas[index].zip != 0) {
            index = (index + 1) % MOD;
        }
        areas[index] = a;
        k++;

        free(copy);
    }

    fclose(ftpr);
    free(lineptr);

    postnr->areas = areas;
    postnr->n = k;

    return postnr;

}

void free_codes(codes *postnr) {
    if (!postnr) return;

    for (int i = 0; i < MOD; i++) {
        if (postnr->areas[i].name != NULL) {
            free(postnr->areas[i].name);
        }
    }

    free(postnr->zips);
    free(postnr->areas);
    free(postnr);
}

bool look_up(codes *postnr, int zip) {
    int index = zip % MOD;
    while (postnr->areas[index].zip != 0) {
        if (postnr->areas[index].zip == zip) {
            return true;
        }
        index = (index + 1) % MOD;
    }
    return false;
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

void collisions(codes *postnr, int mod) {
    int mx = 20;

    int data[mod];
    int cols[mx];

    for (int i = 0; i < mod; i++) {
        data[i] = 0;
    }

    for(int i = 0; i < mx; i++) {
        cols[i] = 0;
    }

    for (int i = 0; i < postnr->n; i++) {
        int index = (postnr->zips[i]) % mod;
        data[index]++;
    }

    int sum = 0;
    for(int i = 0; i < mod; i++) {
        sum += data[i];
        if (data[i] < mx) {
            cols[data[i]]++;
        }
    }

    printf("%d (%d) : ", mod, sum);
    for (int i = 1; i < mx; i++) {
        printf("%6d ", cols[i]);
    }
    printf("\n");
}

int main()  {
    struct timespec start, stop;

    codes *stockholm = read_postcodes("postnummer.csv");
    if (!stockholm) {
        printf("Failed to read postcodes\n");
        return 1;
    }

    collisions(stockholm, 10000);
    collisions(stockholm, 20000);
    collisions(stockholm, 12345);
    collisions(stockholm, 17389);
    collisions(stockholm, 13513);  // prime
    collisions(stockholm, 13600);  // not prime  
    collisions(stockholm, 14000);  // not prime

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
