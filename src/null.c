#include <stdlib.h>

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

int main() {
    //dereferenceNull();
    //forgetToFree();
    data100();
    return 0;
}