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

long search(int n, int loop) {
    struct timespec t_start, t_stop;
    
    int *array = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) array[i] = rand()%(n*2);

    int *keys = (int*)malloc(loop*sizeof(int));
    for (int i = 0; i < loop; i++) keys[i] = rand()%(n*2);

    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC, &t_start);
    for (int i = 0; i < loop; i++) {
        int key = keys[i];
        for (int j = 0; j < n; j++) 
        {if (key == array[j]) {
            sum++;
            break;
        }
    }
}
clock_gettime(CLOCK_MONOTONIC, &t_stop);
long wall = nano_seconds(&t_start, &t_stop);
return wall;
}

int main(int argc, char *argv[]) {
    int sizes[] = {1000,2000,4000,8000,16000,32000};
    int k = 10;
    int loop = 1000;
    for (int i = 0; i < 6; i++) {
        int n = sizes[i];
        long min = LONG_MAX;
        for (int i = 0; i < k; i++) {
            long wall = search(n, loop);
            if (wall < min)
              min = wall;
    }
    printf("%d %0.2f ns\n", n, (double)min/loop); 
  }
}