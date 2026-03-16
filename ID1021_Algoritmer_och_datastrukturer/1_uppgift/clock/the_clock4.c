#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

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
    long wall = nano_seconds(&t_start, &t_stop);
    return wall;
}

int main() {
for (int i = 0; i < 10; i++) {
long wall = bench(10, 5);
printf("time : %ld ns\n", wall);
}
}