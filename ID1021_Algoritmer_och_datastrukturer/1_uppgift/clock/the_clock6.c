#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

long nano_seconds(struct timespec *t_start, struct timespec *t_stop) 
{
    return (t_stop ->tv_nsec - t_start ->tv_nsec) + 
    (t_stop ->tv_sec - t_start ->tv_sec)*1000000000L;
}

long bench(int n, int loop) {
    struct timespec t_start, t_stop;
    
    int *array = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array[i] = i;

    int *indx = (int*)malloc(loop*sizeof(int));
    for (int i = 0; i < loop; i++) indx[i] = rand()%n;

    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++) sum += array[indx[i]];
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    if (sum == 0)
        return 0;
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}

int main(int argc, char *argv[]) {
    int n = 1000;
    int loop = 1000;
    int k = 10;

    long min = LONG_MAX;
    long max = 0;
    long total = 0;

    for (int i = 0; i < k; i++) {
        long wall = bench(n, loop);
        if (wall < min) min = wall;
    }

    printf("minimum time: %0.2f ns/operation \n", (double)min/loop);
    
    return 0;
}
