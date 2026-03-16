#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define STADER 73
#define BUFFER 200

struct city;

typedef struct connection {
    struct city *destination;
    int dst;
    struct connection *next;
} connection;

typedef struct city {
    char *name;
    connection *first;
} city;

typedef struct map {
    city *cities[STADER];
} map;

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_sec - t_start->tv_sec) * 1000000000L +
            (t_stop->tv_nsec - t_start->tv_nsec);
}


void connect (city *src, city *des, int dst) {
    connection *stad = malloc(sizeof(connection));
    if (stad == NULL) return;

    stad->destination = des;
    stad->dst = dst;
    stad->next = src->first;

    src->first = stad;
}

int hash(char *name, int mod) {

    int h = 0;
    int i = 0;
    unsigned char c = 0;

    while ((c = name[i]) != 0) {
        h = (h*31 + c) % mod;
        i++;
    }

    return h;    
}


city *look_up(map *cou, char *name) {
    int index = hash(name, STADER);
    while (cou->cities[index] != NULL) {
        int cmp = strcmp(cou->cities[index]->name, name);
        if (cmp == 0) {
            return cou->cities[index];
        } else {
            index = (index + 1) % STADER;
        }
    }

    city *stad = malloc(sizeof(city));
    if (stad == NULL) return NULL;
    stad->name = strdup(name);
    stad->first = NULL;

    cou->cities[index] = stad;

    return stad;
}

map *graph(char *file) {
    map *trains = (map*)malloc(sizeof(map));

    FILE *fptr = fopen(file, "r");

    if (fptr == NULL) return NULL;

    char *lineptr = malloc(sizeof(char) * BUFFER);

    size_t n = BUFFER;
    while(getline(&lineptr, &n, fptr) > 0) {
        char *copy = (char*)malloc(sizeof(char)*(strlen(lineptr)+1));
        strcpy(copy, lineptr);

        city *src = look_up(trains,strtok(copy, ","));
        city *des = look_up(trains, strtok(NULL, ","));
        int dist = atoi(strtok(NULL, ","));

        connect(src, des, dist);
        connect(des, src, dist);
    }
    fclose(fptr);
    return trains;

}

bool loop(city *stad, city **path, int k) {
    for (int i = 0; i < k; i++) {
        int cmp = strcmp(path[i]->name, stad->name);
        if (cmp == 0) return true;
    }
    return false;
}

int shortest_path(city *from, city *to, city **path, int k, int sofar) {

    if (from == to) {
        return 0;
    }


    connection *nxt = from->first;

    while(nxt != NULL) {
        int limit = (sofar == -1) ? -1 : sofar - nxt->dst;

        if (!loop(nxt->destination, path, k) && (sofar == -1 || nxt->dst < sofar) ) {
            path[k] = from;

            int d = shortest_path(nxt->destination, to, path, k+1, limit);
            if (d >= 0 && ((sofar == -1) || (d + nxt->dst) < sofar)){
                sofar = (d+nxt->dst);
            } 
        }
         nxt = nxt->next;
    }
    return sofar;
}

int main(int argc, char*argv[]) {

    map *trains = graph("trains.csv");
    if (trains == NULL) return -1;

    if(argc < 3) {
        printf("usage: %s <from> <to> <limit>\n", argv[0]);
        exit(1);
    }

    city *from = look_up(trains, argv[1]);
    city *to = look_up(trains, argv[2]);
    city **path = malloc(sizeof(city*)*STADER);

    int k = 0;

    struct timespec t_start, t_stop;

    clock_gettime(CLOCK_MONOTONIC, &t_start);

    int s = shortest_path(from, to, path, k, -1);

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);

    if (s > 0) {
        printf("shortest path %d found in %.2fms\n", s, ((double)wall)/1000000);
    } else {
        printf("no path found\n");
    }

    return 0;
}

/* 
Malmö Lund
shortest path 13 found in 0.00ms

Malmö Helsingborg
shortest path 48 found in 0.00ms

Malmö Hässleholm
shortest path 43 found in 0.01ms

Malmö Göteborg
shortest path 153 found in 0.11ms

Malmö Stockholm
shortest path 273 found in 0.52ms

Malmö Sundsvall
shortest path 600 found in 37.35ms

Malmö Umeå
shortest path 790 found in 217.94ms

Malmö Boden
shortest path 976 found in 669.85ms

Malmö Kiruna
shortest path 1162 found in 1287.03ms

*/