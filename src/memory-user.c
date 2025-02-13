#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int main() {
    int megabytes;
    int duration_seconds;
    printf("Input the number of megabytes to allocate, and the time period in seconds to run through them, separated by a comma\n");
    scanf("%d,%d", &megabytes, &duration_seconds);
    printf("Allocating %d megabytes for %d seconds...\n", megabytes, duration_seconds);

    time_t start_time = time(NULL);

    int* ptr = (int*)calloc(megabytes * 1024 * 1024 / sizeof(int), sizeof(int));
    if (!ptr) {
        printf("Error, ptr is null\n");
    }
    while (ptr && (time(NULL) - start_time < duration_seconds)) {
        for (int i = 0; i < megabytes * 1024 * 1024 / sizeof(int); i += 4096 / sizeof(int)) {
            ptr[i] = 10 + i;
        }
        for (int i = 0; i < megabytes; ++i) {
            printf("Value at index %d is found to be: %d\n", i, ptr[i]);
        }
    }
    free(ptr);
    printf("Memory freed\n");
    return 0;
}