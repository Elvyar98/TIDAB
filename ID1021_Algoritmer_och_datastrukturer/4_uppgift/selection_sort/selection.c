#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) + 
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000L;
}

void selection_sort(int array [], int n) {
    for (int i = 0; i < n -1; i++) {
        int candidate = array[i];
        int position = i;
         for (int j = i + 1; j < n; j++) {

            if (array[j] < candidate) {
                candidate = array[j];
                position = j;
            }
        }

        if (i != position) {
        int p = array[i];
        array[i] = candidate;
        array[position] = p;
        }

    }
}


int *unsorted(int n) {
    int *array = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array[i] = rand()%(n*2);
    return array;
}

int main(int argc, char *argv[]) {
    int sizes[] = {1000, 2000, 4000, 8000};
    int k = 10;      // Number of benchmark repetitions
    int sorts_per_test = 100;  // Number of searches per benchmark run
    
    struct timespec start, end;
    
    printf("Array Size\tAverage Time (ms)\tMin Time (ms)\n");
    printf("====================================================\n");
    
    // Loop through different array sizes
    for (int size_idx = 0; size_idx < 4; size_idx++) {
        int n = sizes[size_idx];
        
        // Allocate and initialize array
        
        
        long min_time = LONG_MAX;
        long total_time = 0;
        
        // Run benchmark k times
        for (int rep = 0; rep < k; rep++) {
            // Time a batch of sorts
            clock_gettime(CLOCK_MONOTONIC, &start);
            
            // Perform multiple sorts
            for (int sort = 0; sort < sorts_per_test; sort++) {
                int *array = unsorted(n);
                selection_sort(array, n);
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