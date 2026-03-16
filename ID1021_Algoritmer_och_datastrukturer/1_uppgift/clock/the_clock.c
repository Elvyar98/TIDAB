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
    long sum = 0;
    for(int i = 0; i < 10; i++) {
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        long wall = nano_seconds(&t_start, &t_stop);
        sum += wall;
        printf("%ld ns\n", wall);

    }
    long avg = sum / 10;
    printf("Average time is %ld ns", avg);
    
    return 0;

}

/* Programmet räknar i nanosekunder tiden mellan de två clock_gettime 
funktionerna*/