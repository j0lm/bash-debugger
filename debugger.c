#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

pid_t execute_command(const char *cmd, int pipe_fd[2]) {
    pid_t pid = fork();
    if (pid == -1) {
	perror("fork failed");
    }
    
    if (pid == 0) {  // child process
        close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	dup2(pipe_fd[1], STDERR_FILENO);
	close(pipe_fd[1]);

	execlp("bash", "bash", "-c", cmd, NULL);
	perror("error executing script");
	exit(1);
    } 
    return pid;
}

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

void cleanup_ncurses() {
    printw("\nPress any key to exit...");
    getch();
    endwin();
}

void read_pipe_and_display(int pipe_fd[2]) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) -1)) > 0) {
        buffer[bytes_read] = '\0';
        printw("%s", buffer);
        refresh();

	if (getch() == 'q') {
            return;
        }
    }
}

int main () {
    int pipe_fd[2];
    if(pipe(pipe_fd) == -1) {
        perror("error executing pip()");
	exit(1);
    }

    pid_t child_pid = execute_command("ls -l", pipe_fd);

    close(pipe_fd[1]);

    init_ncurses();
    read_pipe_and_display(pipe_fd);
    close(pipe_fd[0]);
    waitpid(child_pid, NULL, 0);

    cleanup_ncurses();
    return 0;
}
