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

int shortest(city *from, city *to, int left) {

    if (from == to) {
        return 0;
    }

    int sofar = -1;

    connection *nxt = from->first;

    while(nxt != NULL) {
        if (nxt->dst <= left) {
            int d = shortest(nxt->destination, to, left - nxt->dst);
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

    if(argc < 4) {
        printf("usage: %s <from> <to> <limit>\n", argv[0]);
        exit(1);
    }

    city *from = look_up(trains, argv[1]);
    city *to = look_up(trains, argv[2]);
    int limit = atoi(argv[3]);

    struct timespec t_start, t_stop;

    clock_gettime(CLOCK_MONOTONIC, &t_start);

    int s = shortest(from, to, limit);

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
Malmö Göteborg 300
shortest path 153 found in 1.53ms

Göteborg Stockholm 300
shortest path 211 found in 1.05ms

Malmö Stockholm 300
shortest path 273 found in 1.40ms

Stockholm Sundsvall 300
no path found

Stockholm Sundsvall 500
shortest path 327 found in 1347.15ms

Stockholm Umeå 300
no path found

Göteborg Sundsvall 300
no path found

Sundsvall Umeå 300
shortest path 190 found in 0.00ms

Göteborg Umeå 300
no path found

Umeå Göteborg 300
no path found

*/