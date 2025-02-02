#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void forkWait() {
    printf("Hello, WSL!\n");
    int x = 100;

    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child
        //x = 73;
        printf("child (pid:%d)\n", (int) getpid());
        //printf("value of x: %d\n", x);
    } else {
        // Parent
        int status;
        int child_pid = waitpid(pid, &status, 0);

        if (child_pid == -1) {
            perror("waitpid)");
            exit(1);
        }
        if (WIFEXITED(status)) {
            printf("Child %d exited with status %d\n", child_pid, WEXITSTATUS(status));
        } else {
            printf("Child %d did not exit normally\n", child_pid);
        }
        //printf("parent of %d (child_pid:%d) (pid:%d)\n", pid, child_pid, (int) getpid());
        //printf("value of x: %d\n", x);
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

void execTest() {
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

void closeStdout() {
    printf("Starting closeStdout...\n");
    int pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // Child
        printf("Closing stdout...\n");
        fclose(stdout);
        printf("I think this shouldn't print\n");
    } else {
        // Parent
        int child_pid = wait(NULL);
        printf("I'm the parent, will I print?\n");
    }
    freopen("/dev/tty", "w", stdout);
    printf("This should print\n");
}

void forkPipe() {
    int fd[2];
    pipe(fd);

    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid_t pid1, pid2;
    pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid1 == 0) {
        // Child1
        close(fd[0]); // Close unused read end
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout to pipe's write end
        close(fd[1]); // Close original write end after duplication

        // stdout now goes into the pipe
        printf("Hello from child1!\n");
        exit(0);
    } else {
        // Parent
        pid2 = fork();
        
        if (pid2 < 0) {
            fprintf(stderr, "second fork failed\n");
            exit(1);
        } else if (pid2 == 0) {
            // Child2
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            char buffer[100];
            while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                printf("Child received: %s", buffer);
            }
            exit(0);
        } else {
            // Runs in the original parent process
            close(fd[0]);
            close(fd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
}

int main() {
    // printf("Fork 1 starting...\n");
    //forkWait();
    // printf("Fork openFile starting...\n");
    // forkOpenFile();
    //execTest();
    //closeStdout();
    forkPipe();
    return 0;
}