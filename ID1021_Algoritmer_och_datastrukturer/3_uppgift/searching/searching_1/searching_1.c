#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) {
    return (t_stop->tv_nsec - t_start->tv_nsec) + 
           (t_stop->tv_sec - t_start->tv_sec) * 1000000000L;
}

bool unsorted_search(int array[], int length, int key) {
    for (int index = 0; index < length; index++) {
        if (array[index] == key) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 32000, 1000000};
    int k = 10;      // Number of benchmark repetitions
    int searches_per_test = 1000;  // Number of searches per benchmark run
    
    struct timespec start, end;
    
    printf("Array Size\tAverage Time (ms)\tMin Time (ms)\n");
    printf("====================================================\n");
    
    // Loop through different array sizes
    for (int size_idx = 0; size_idx < 7; size_idx++) {
        int n = sizes[size_idx];
        
        // Allocate and initialize array
        int *array = (int*)malloc(n * sizeof(int));
        if (array == NULL) {
            fprintf(stderr, "Memory allocation failed for size %d\n", n);
            continue;
        }
        
        // Fill array with random values
        for (int i = 0; i < n; i++) {
            array[i] = rand() % (n * 2);
        }
        
        long min_time = LONG_MAX;
        long total_time = 0;
        
        // Run benchmark k times
        for (int rep = 0; rep < k; rep++) {
            // Time a batch of searches
            clock_gettime(CLOCK_MONOTONIC, &start);
            
            // Perform multiple searches
            for (int search = 0; search < searches_per_test; search++) {
                int key = rand() % (n * 2);  // Random key to search
                unsorted_search(array, n, key);
            }
            
            clock_gettime(CLOCK_MONOTONIC, &end);
            
            long elapsed = nano_seconds(&start, &end);
            long time_per_search = elapsed / searches_per_test;
            
            if (time_per_search < min_time) {
                min_time = time_per_search;
            }
            total_time += time_per_search;
        }
        
        double avg_time = (double)total_time / k;
        
        printf("%9d\t%16.2f\t%14.2f\n", 
               n, avg_time/1000, (double)min_time/1000);
        
        free(array);
    }
    
    return 0;
}