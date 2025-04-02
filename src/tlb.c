#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int argc;
    char** argv;
} ThreadArgs;

void* runMeasurements(void* thread_args) {
    ThreadArgs* args = (ThreadArgs*)thread_args;
    int argc = args->argc;
    char** argv = args->argv;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s NUMPAGES\n", argv[0]);
        return NULL;
    }

    int LOOPREPS = 1000;
    int NUMPAGES = atoi(argv[1]);
    int PAGESIZE = getpagesize();

    int totalElapsedNs = 0;

    pthread_t current_thread = pthread_self();

    // CPU mask - pins to CPU core 0
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    // Set thread affinity
    int result = pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
    if (result != 0) {
        fprintf(stderr, "Error setting thread affinity %d\n", result);
        return NULL;
    }

    
    for (int j = 0; j < LOOPREPS; ++j) {
        int jump = PAGESIZE / sizeof(int);
        int i;
        
        volatile int* a = malloc(NUMPAGES * PAGESIZE * sizeof(int));
        if (a == NULL) {
            perror("malloc failed");
            return NULL;
        }

        // Initialize loop values to 0
        for (int k = 0; k < NUMPAGES * PAGESIZE; k++) {
            a[k] = 0;
        }

        struct timespec start, end;
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &start) == -1) {
            perror("clock_gettime start");
            free((void*)a);
            return NULL;
        }
        
        // ---------------
        for (i = 0; i < NUMPAGES * PAGESIZE; i += jump) {
            a[i] += 1;
        }
        // ---------------

        if (clock_gettime(CLOCK_MONOTONIC_RAW, &end) == -1) {
            perror("clock_gettime end");
            free((void*)a);
            return NULL;
        }
        long elapsedNs = (end.tv_sec - start.tv_sec) * 1000000000L +
        (end.tv_nsec - start.tv_nsec);
        totalElapsedNs += elapsedNs; 
        //printf("Elapsed time: %ld nanoseconds\n", elapsed_ns);
        
        free((void*)a);
    }
    int avgElapsedNs = totalElapsedNs / LOOPREPS;
    
    printf("Average elapsed time: %d nanoseconds\n", avgElapsedNs);
    
    return NULL;
}

int main(int argc, char* argv[]) {

    pthread_t thread;

    ThreadArgs args;
    args.argc = argc;
    args.argv = argv;

    if (pthread_create(&thread, NULL, runMeasurements, &args)) {
        perror("pthread_create");
        return 1;
    }

    pthread_join(thread, NULL);

    return 0;
}