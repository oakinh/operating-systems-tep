#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Hello, WSL!\n");

    FILE* file = fopen("../monster.txt", "w");

    if (!file) {
        printf("The file is not opened. The program will now exit.\n");
        exit(0);
    }

    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child
        printf("child (pid:%d)\n", (int) getpid());
    } else {
        // Parent
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait:%d) (pid:%d)\n", pid, rc_wait, (int) getpid());
    }
    return 0;
}