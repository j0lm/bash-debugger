#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#define BUFFER_SIZE 1024

void setup_ncurses();
void cleanup_ncurses();
void read_pipe_and_display(int fd);
void run_child_process(int pipe_fd[2]);
void run_parent_process(int pipe_fd[2]);

int main() {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {  
        run_child_process(pipe_fd);
    } else {  
        run_parent_process(pipe_fd);
    }

    return 0;
}

void setup_ncurses() {
    initscr();              
    cbreak();               
    noecho();               
    scrollok(stdscr, TRUE); 
}

void cleanup_ncurses() {
    getch();    
    endwin();   
}

void read_pipe_and_display(int fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; 
        printw("%s", buffer);      
        refresh();                 
    }
}

void run_child_process(int pipe_fd[2]) {
    close(pipe_fd[0]);  

    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
        perror("dup2 (stdout) failed");
        exit(1);
    }

    if (dup2(pipe_fd[1], STDERR_FILENO) == -1) {  
        perror("dup2 (stderr) failed");
        exit(1);
    }

    close(pipe_fd[1]);  

    execlp("ls", "ls", "-l", NULL);  
    perror("execlp failed");
    exit(1);
}

void run_parent_process(int pipe_fd[2]) {
    close(pipe_fd[1]);  

    setup_ncurses();
    read_pipe_and_display(pipe_fd[0]);  
    cleanup_ncurses();

    close(pipe_fd[0]);  
}

