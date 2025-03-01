#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) { // Child process
        close(pipe_fd[0]); // Close read end

        FILE *log = fopen("/tmp/debug.log", "w");
        fprintf(log, "DEBUG: pipe_fd[1] = %d\n", pipe_fd[1]);
        fflush(log);

        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            fprintf(log, "dup2 failed: %s\n", strerror(errno));
            fflush(log);
            exit(1);
        }

        fprintf(log, "dup2 succeeded!\n");
        fflush(log);
        fclose(log);

        printf("This should be redirected\n");
        fflush(stdout);

        exit(0);
    }

    sleep(1);
    close(pipe_fd[1]);

    char buffer[100];
    read(pipe_fd[0], buffer, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    printf("Parent received: %s\n", buffer);
    return 0;
}

