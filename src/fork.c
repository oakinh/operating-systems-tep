#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void fork1() {
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
}

void forkOpenFile() {
    freopen("output.txt", "a+", stdout); 
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child
        printf("child (pid:%d)\n", (int) getpid());
    } else {
        // Parent
        //int rc_wait = wait(NULL);
        printf("parent of %d (pid:%d)\n", pid, (int) getpid());
    }
    freopen("/dev/tty", "w", stdout); /*for gcc, ubuntu*/  
}

int execTest() {
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child
        char *execArgs[3];
        execArgs[0] = strdup("wc");
        execArgs[1] = strdup("output.txt");
        execArgs[2] = NULL;
        execvp(execArgs[0], execArgs);
    } else {
        wait(NULL);
        printf("parent finished\n");
    }
}

int main() {
    // printf("Fork 1 starting...\n");
    // fork1();
    // printf("Fork openFile starting...\n");
    // forkOpenFile();
    execTest();
    return 0;
}