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

int main() {
    struct timespec t_start, t_stop;
    int array[] = {0,1,2,3,4,5,6,7,8,9};
    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < 1000; i++) {
        sum += array[rand()%10];
    } 
    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    long wall = nano_seconds(&t_start, &t_stop);
    printf("%ld ms\n", wall/1000);

    return 0;
}

/* Mäter tiden det tar för en loop att köra*/