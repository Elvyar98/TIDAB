#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define AREAS 10000
#define BUFFER 200
#define MOD 17389

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_sec - t_start->tv_sec) * 1000000000L +
            (t_stop->tv_nsec - t_start->tv_nsec);
}

typedef struct area {
    char *name;
    int zip;
    int pop;
} area;

typedef struct node
{
    area a;
    struct node *next;
} node;

typedef struct bucket {
    node *first;
} bucket;

typedef struct codes {
    bucket *buckets;
    int *zips;
    int n;
} codes;


bucket *create_buckets(int mod) {
    bucket *buckets = (bucket*)malloc(sizeof(bucket) * mod);
    if (!buckets) return NULL;

    for (int i = 0; i < mod; i++) {
        buckets[i].first = NULL;
    }

    return buckets;
} 

void add_bucket(bucket *buc, area a) {
    node *new = (node*)malloc(sizeof(node));
    if (!new) return;

    new->a = a;
    new->next = buc->first;
    buc->first = new;

}

codes *read_postcodes(char *file) {
    codes *postnr = (codes*)malloc(sizeof(codes));
    if (!postnr) return NULL;

    bucket *buckets = create_buckets(MOD);
    if (!buckets) {
        free(postnr);
        return NULL;
    }

    postnr->buckets = buckets;

    int *zips = (int*)malloc(sizeof(int) * AREAS);
    if (!zips) {
        free(buckets);
        free(postnr);
        return NULL;
    }
    postnr->zips = zips;

    FILE *ftpr = fopen(file, "r");
    if (!ftpr) {
        free(buckets);
        free(postnr);
        free(zips);
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
        add_bucket(&postnr->buckets[index], a);
        k++;

        free(copy);
    }

    fclose(ftpr);
    free(lineptr);

    postnr->n = k;

    return postnr;

}

void free_buckets(bucket *buckets, int mod) {
    for (int i = 0; i < mod; i++) {
        node *current = buckets[i].first;
        while (current != NULL) {
            node *next = current->next;
            free(current->a.name);
            free(current);
            current = next;
        }
    }
    free(buckets);
}

void free_codes(codes *postnr) {
    if (!postnr) return;
    free_buckets(postnr->buckets, MOD);
    free(postnr->zips);
    free(postnr);
}

bool look_up(bucket *buckets, int zip) {
    int index = zip % MOD;

    node *current = buckets[index].first;

    while (current != NULL) {
        if (current->a.zip == zip) {
            return true;
        }

        current = current->next;
    }

    return false;

}



long benchmark_search(codes *postnr, int zip, int iterations) {
    long min_time = LONG_MAX;

    for (int i = 0; i < iterations; i++) {
        struct timespec start, stop;
        clock_gettime(CLOCK_MONOTONIC, &start);
        look_up(postnr->buckets, zip);
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

    collisions(stockholm, MOD);

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