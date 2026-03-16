#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) + 
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000L;
}

typedef struct node {
    int value;
    struct node *next;
} node;

typedef struct queue {
    node *first;
    node *last;
} queue;

queue *create_queue() {
    queue *q = (queue*)malloc(sizeof(queue));
    if (q != NULL) {
        q->first = NULL;
        q->last = NULL;
    }
    
    return q;
}

bool empty(queue *q) {
    return (q->first == NULL);
}

void enqueue(queue *q, int v) {
    node *nd = (node*)malloc(sizeof(node));
    if (nd == NULL) return;

    nd->value = v;
    nd->next = NULL;


    if (empty(q)) {
        q->first = nd;
        q->last = nd;
    } else {
        q->last->next = nd;
        q->last = nd;
    }
    
}

int dequeue(queue *q) {
    if (empty(q)) {
        return 0;
    }

    node *temp = q->first;
    int res = temp->value;

    q->first = q->first->next;

    if (q->first == NULL) {
        q->last = NULL;
    }

    free(temp);
    return res;
}

void free_queue(queue *q) {
    if (empty(q)) return;

    node *temp = q->first;
    while (temp != NULL) {
        node *tem = temp;
        temp = temp->next;
        free(tem);
    }
    
    free(q);
}

int main() {

    int sizes[] = {10000, 20000, 40000, 80000};
    int k = 10;               // antal mätta körningar
    int warmup_runs = 3;      // antal warm-up körningar

    srand(time(NULL));

    struct timespec start, stop;

    printf("Size\tAverage Enqueue (ms)\tMin Enqueue (ms)\n");
    printf("====================================================\n");
    
    for (int array_size = 0; array_size < 4; array_size++) {

        int n = sizes[array_size];
        
        long min_time = LONG_MAX;
        long total_time = 0;

        // -------- WARM-UP (ej mätt) --------
        for (int warmup = 0; warmup < warmup_runs; warmup++) {

            queue *a = create_queue();
            
            for (int j = 1; j <= n; j++) {
                enqueue(a,j);
            }

            while (!empty(a)) {
                dequeue(a);
            }

            free(a);
        }

        // -------- MÄTNING --------
        for (int rep = 0; rep < k; rep++) {

            queue *a = create_queue();
            
            clock_gettime(CLOCK_MONOTONIC, &start);

            for (int j = 1; j <= n; j++) {
                enqueue(a, rand() % (j * 2));
            }

            clock_gettime(CLOCK_MONOTONIC, &stop);

            long elapsed = nano_seconds(&start, &stop);
            total_time += elapsed;

            if (elapsed < min_time) {
                min_time = elapsed/1000;
            }

            // Frigör noder
            while (!empty(a)) {
                dequeue(a);
            }

            free(a);
        }
        
        double avg_time = (double) (total_time / k)/1000;

        printf("%9d\t%20.2f\t%18ld\n", n, avg_time, min_time);
    }

    return 0;
}