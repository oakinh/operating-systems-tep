#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Hello, WSL!\n");
    int x = 100;

    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child
        x = 73;
        printf("child (pid:%d)\n", (int) getpid());
        printf("value of x: %d\n", x);
    } else {
        // Parent
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait:%d) (pid:%d)\n", pid, rc_wait, (int) getpid());
        printf("value of x: %d\n", x);
    }
    return 0;
}