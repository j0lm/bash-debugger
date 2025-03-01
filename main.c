#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 256

void setup_ncurses();
void cleanup ncurses();
void read_pipe_and_display(int read_fd)
void execute_command

pid_t execute_command(const char *cmd, int pipe_fd[2]) {
    printf("DEBUG: forking process...\n");
    pid_t pid = fork();
    if (pid == -1) {
	perror("fork failed");
    }
    
    if (pid == 0) {  // ]child process
	printf("Child process runnning with PID: %d\n", getpid());
	fflush(stdout);
	sleep(1);
        close(pipe_fd[0]);
	printf("DEBUG: About to run dup2\n");
	fflush(stdout);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
	    perror("dup2 failed");
	    exit(1);
	}
	dup2(pipe_fd[1], STDERR_FILENO);
	close(pipe_fd[1]);
	printf("Executing: %s\n", cmd);
	fflush(stdout);

	execlp("/bin/sh", "sh", "-c", cmd, NULL);
	perror("error executing script");
	exit(1);
    } 
    
    printf("DEBUG: pid: %d\n", pid);
    fflush(stdout);
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

void read_pipe_and_display(int read_fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(read_fd, buffer, sizeof(buffer) -1)) > 0) {
        buffer[bytes_read] = '\0';
        printw("%s", buffer);
        refresh();

	if (getch() == 'q') {
            return;
        }
	napms(10);
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
    read_pipe_and_display(pipe_fd[0]);
    close(pipe_fd[0]);
    waitpid(child_pid, NULL, 0);

    cleanup_ncurses();
    return 0;
}
