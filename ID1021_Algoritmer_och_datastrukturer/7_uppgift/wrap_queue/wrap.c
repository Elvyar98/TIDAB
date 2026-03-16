#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define MIN 4

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) + 
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000L;
}


typedef struct queue {
    int first;
    int last;
    int size;
    int vals;
    int *array;
} queue;

queue *create_queue() {
    int *array = (int*)malloc(MIN*sizeof(int));
    queue *q = (queue*)malloc(sizeof(queue));
    if (q != NULL) {
        q->size = MIN;
        q->vals = 0;
        q->first = 0;
        q->last = 0;
        q->array = array;
    }
    
    return q;
}

bool empty(queue *q) {
    return (q->vals == 0);
}

void enqueue(queue *q, int v) {
    if (q->vals == q->size){
        int size = q->size * 2;
        int *copy = (int*)malloc(size*sizeof(int));
        for (int i = 0; i < q->vals; i++){
            copy[i] = q->array[(q->first + i) % q->size];
        }

        free(q->array);
        q->array = copy;
        q->size = size;
        q->first = 0;
        q->last = q->vals;
    }

    q->array[q->last] = v;
    q->last = (q->last + 1) % q->size;
    q->vals++;   
}

int dequeue(queue *q) {
   if (empty(q)){
        printf("Queue underflow\n");
        return -1;
    }

    int val = q->array[q->first];
    q->first = (q->first + 1) % q->size;
    q->vals--;

    if ( q->vals > 0 && q->vals <= q->size/4){
        int size = q->size/2;
        int *copy = (int*)malloc(size*sizeof(int));


        for (int i = 0; i < q->vals; i++){
            copy[i] = q->array[(q->first + i) % q->size];
        }

        free(q->array);
        q->array = copy;
        q->size = size;
        q->first = 0;
        q->last = q->vals;
    }

    return val;
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
                enqueue(a, j);
            }

            while (!empty(a)) {
                dequeue(a);
            }

            free(a->array);
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
                min_time = elapsed;
            }

            free(a->array);
            free(a);
        }
        
        double avg_time = (double) (total_time / k)/1000;
        double min_time_ms = (double) min_time / 1000;

        printf("%9d\t%20.2f\t%20.2f\n", n, avg_time, min_time_ms);
    }

    return 0;
}

