#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) + 
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000L;
}



void merge(int *array, int *aux, int lo, int mid, int hi) {
    for (int k = lo; k <=hi; k++){
        aux[k] = array[k];
    }

    int i = lo;
    int j = mid+1;

    for (int k = lo; k <=hi; k++) {
        if (i > mid) {
            array[k] = aux[j++];
        } else if (j > hi) {
            array[k] = aux[i++];
        } else if (aux[i] <= aux[j]) {
            array[k] = aux[i++];
        } else {
            array[k] = aux[j++];
        }
    }
}

void merge_sort(int *array, int *aux, int lo, int hi) {
    if (lo != hi) {
        int mid = (lo + hi)/2;
        merge_sort(array, aux, lo, mid);
        merge_sort(array, aux, mid+1, hi);
        merge(array, aux, lo, mid, hi);
    }
}

void sort(int *array, int n) {
    if (n == 0)
       return;
    
    int *aux = (int*)malloc(n * sizeof(int));
    merge_sort(array, aux, 0, n-1);
    free(aux);
}


int *unsorted(int n) {
    int *array = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array[i] = rand()%(n*2);
    return array;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int sizes[] = {1000, 2000, 4000, 8000};
    int k = 10;      // Number of benchmark repetitions
    int sorts_per_test = 100;  // Number of searches per benchmark run
    
    struct timespec start, end;
    
    printf("Array Size\tAverage Time (ms)\tMin Time (ms)\n");
    printf("====================================================\n");
    
    // Loop through different array sizes
    for (int size_idx = 0; size_idx < 4; size_idx++) {
        int n = sizes[size_idx];
        
        
        long min_time = LONG_MAX;
        long total_time = 0;
        
        // Run benchmark k times
        for (int rep = 0; rep < k; rep++) {
            // Time a batch of sorts
            clock_gettime(CLOCK_MONOTONIC, &start);
            
            // Perform multiple sorts
            for (int sorts = 0; sorts < sorts_per_test; sorts++) {
                int *array = unsorted(n);
                sort(array, n);
                free(array);
            }
            
            clock_gettime(CLOCK_MONOTONIC, &end);
            
            long elapsed = nano_seconds(&start, &end);
            long time_per_sort = elapsed / sorts_per_test;
            
            if (time_per_sort < min_time) {
                min_time = time_per_sort;
            }
            total_time += time_per_sort;
        }
        
        double avg_time = (double)total_time / k;
        
        printf("%9d\t%16.3f\t%14.3f\n", 
               n, avg_time/1000000.0, (double)min_time/1000000.0);
        
    }
    
    return 0;
}