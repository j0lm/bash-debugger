#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/wait.h>

int main () {
    int pipe_fd[2];
    if(pipe(pipe_fd) == -1) {
        perror("error executing pip()");
	return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
	perror("error executing fork ()");
    }
    
    if (pid == 0) {  // child process
        close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(pipe_fd[1], STDERR_FILENO);
	close(pipe_fd[1]);

	execlp("bash", "bash", "./test.sh", NULL);
	perror("error executing script");
	exit(1);
    } else { // parent process
        close(pipe_fd[1]);
	char buffer[256];
	ssize_t bytes_read;

	while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) -1)) > 0) {
            buffer[bytes_read] = '\0';
	    printf("%s", buffer);
	}

	close(pipe_fd[0]);
	wait(NULL);
    }
    return 0;
}
