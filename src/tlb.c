#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s NUMPAGES\n", argv[0]);
        return 1;
    }

    int LOOPREPS = 100;
    int NUMPAGES = atoi(argv[1]);
    int PAGESIZE = getpagesize();

    int totalElapsedNs = 0;
    
    for (int j = 0; j < LOOPREPS; ++j) {
        int jump = PAGESIZE / sizeof(int);
        int i;
        
        volatile int* a = malloc(NUMPAGES * PAGESIZE * sizeof(int));
        if (a == NULL) {
            perror("malloc failed");
            return 1;
        }

        // Initialize loop values to 0
        for (int k = 0; k < NUMPAGES * PAGESIZE; k++) {
            a[k] = 0;
        }

        struct timespec start, end;
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &start) == -1) {
            perror("clock_gettime start");
            free((void*)a);
            return 1;
        }
        
        // ---------------
        for (i = 0; i < NUMPAGES * PAGESIZE; i += jump) {
            a[i] += 1;
        }
        // ---------------

        if (clock_gettime(CLOCK_MONOTONIC_RAW, &end) == -1) {
            perror("clock_gettime end");
            free((void*)a);
            return 1;
        }
        long elapsedNs = (end.tv_sec - start.tv_sec) * 1000000000L +
        (end.tv_nsec - start.tv_nsec);
        totalElapsedNs += elapsedNs; 
        //printf("Elapsed time: %ld nanoseconds\n", elapsed_ns);
        
        free((void*)a);
    }
    int avgElapsedNs = totalElapsedNs / LOOPREPS;
    
    printf("Average elapsed time: %d nanoseconds\n", avgElapsedNs);
    
    return 0;
}