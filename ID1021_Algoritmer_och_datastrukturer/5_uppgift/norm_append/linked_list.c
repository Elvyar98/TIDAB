#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) + 
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000L;
}

typedef struct cell {
    int value;
    struct cell *tail;
} cell;

typedef struct linked {
    cell *first;
} linked;

linked *linked_create() {
    linked *new = (linked*)malloc(sizeof(linked));
    new ->first = NULL;
    return new;
}

void linked_free(linked *lnk) {
    cell *nxt = lnk->first;
    while (nxt != NULL) {
        cell *tmp = nxt->tail;
        free(nxt);
        nxt = tmp;
    }
    free(lnk);
}

void linked_add(linked *lnk, int item) {
    cell *new = (cell*)malloc(sizeof(cell));
    if (new == NULL) {
        return;
    }

    new->value = item;
    new->tail = lnk->first;
    lnk->first = new;
}

int linked_length(linked *lnk) {
    int num = 0;
    cell *nxt = lnk->first;
    while (nxt != NULL) {
        num ++;
        nxt = nxt->tail;
    }
    return num;
}

bool linked_find(linked *lnk, int item) {
    if (lnk == NULL) return false;

    cell *current = lnk->first;
    while (current != NULL)
    {
        if (current->value == item) {
            return true;
        }
        current = current->tail;
    }
    return false;
}

void linked_remove(linked *lnk, int item) {
    if (lnk == NULL || lnk->first == NULL) return;

    if (lnk->first->value == item){
        cell *to_remove = lnk->first;
        lnk->first = lnk->first->tail;
        free(to_remove);
        return;
    }

    cell *current = lnk->first;
   
    while (current->tail != NULL) {
        if (current->tail->value == item){
            cell *to_remove = current->tail;
            current->tail = current->tail->tail;
            free(to_remove);
            return;
        }
        current = current->tail;
    }
}

void linked_append(linked *a, linked *b) {
    if (a == NULL || b == NULL) return;

    if (a->first == NULL) {
        a->first = b->first;
    } else {
        cell *current = a->first;
        while (current->tail != NULL)
        {
            current = current->tail;
        }
        current ->tail = b->first;
    }

    b->first = NULL;
}

int main() {
    int sizes[] = {1000, 2000, 4000, 8000};
    int k = 10;               // Number of measured runs
    int warmup_runs = 3;      // Number of warm-up runs

    srand(time(NULL));

    struct timespec start, stop;

    printf("Array Size\tAverage Time (ns)\tMin Time (ns)\n");
    printf("====================================================\n");
    
    for (int array_size = 0; array_size < 4; array_size++) {
        int n = sizes[array_size];
        
        long min_time = LONG_MAX;
        long total_time = 0;

        // Run WARM-UP repetitions (not timed)
        for (int warmup = 0; warmup < warmup_runs; warmup++) {
            linked *list_1 = linked_create();
            linked *list_2 = linked_create();
            
            // Fill list_1 with n random elements
            for (int j = 1; j <= n; j++) {
                linked_add(list_1, rand() % (j * 2));
            }
            
            // Fill list_2 with fixed 1000 elements
            for (int i = 1; i <= 1000; i++) {
                linked_add(list_2, rand() % (i * 2));
            }
            
            // Warm-up append (not timed)
            linked_append(list_1, list_2);
            
            // Clean up
            linked_free(list_2);
            linked_free(list_1);
        }

        // Run MEASURED repetitions
        for (int rep = 0; rep < k; rep++) {
            // Create fresh lists for each repetition
            linked *list_1 = linked_create();
            linked *list_2 = linked_create();
            
            // Fill list_1 with n random elements
            for (int j = 1; j <= n; j++) {
                linked_add(list_1, rand() % (j * 2));
            }
            
            // Fill list_2 with fixed 1000 elements
            for (int i = 1; i <= 1000; i++) {
                linked_add(list_2, rand() % (i * 2));
            }
            
            // Time the append operation
            clock_gettime(CLOCK_MONOTONIC, &start);
            linked_append(list_1, list_2);
            clock_gettime(CLOCK_MONOTONIC, &stop);
            
            long elapsed = nano_seconds(&start, &stop);
            total_time += elapsed;
            
            if (elapsed < min_time) {
                min_time = elapsed;
            }
            
            // Clean up
            linked_free(list_2);
            linked_free(list_1);
        }
        
        double avg_time = (double)total_time / k;
        printf("%9d\t%16.2f\t%14ld\n", n, avg_time, min_time);
    }
    
    return 0;
}