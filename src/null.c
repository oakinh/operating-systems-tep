#include <stdlib.h>
#include <stdio.h>

// Use gdb and valgrind to see the effects of the errors from the functions below

void dereferenceNull() {
    int* ptr;

    ptr = NULL;

    int value = *ptr;
}

void forgetToFree() {
    int* ptr = (int*)malloc(sizeof(int));
}

void data100() {
    int* data = (int*)malloc(100 * sizeof(int));
    data[100] = 0;
}

void dataPrint() {
    int* data = (int*)malloc(100 * sizeof(int));
    free(data);
    printf("Data[57] is: %d", data[57]);
}

void dataPtrFreed() {
    int* data = (int*)malloc(100* sizeof(int));
    int* midData = &data[57];
    free(midData);
}

int main() {
    //dereferenceNull();
    //forgetToFree();
    //data100();
    //dataPrint();
    dataPtrFreed();
    return 0;
}