#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MAX_CMD_LEN 256

void setup_ncurses();
void cleanup_ncurses();
void display_output(int fd);
void run_child_process(int pipe_fd[2], char *command);
void run_parent_process(int pipe_fd[2]);
void get_user_command(char *cmd_buffer);

int main() {
    char command[MAX_CMD_LEN];

    setup_ncurses();

    while(1) {
        get_user_command(command);
        
        if (strcmp(command, "exit") == 0) {
            break;
        }
        
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
            run_child_process(pipe_fd, command);
        } else {
            close(pipe_fd[1]);
            display_output(pipe_fd[0]);
            close(pipe_fd[0]);
        }
    }
    cleanup_ncurses();
    return 0;
}

void setup_ncurses() {
    initscr();              
    cbreak();               
    noecho();               
    scrollok(stdscr, TRUE); 
}

void cleanup_ncurses() {
    mvprintw(LINES -1, 0, "Press any key to exit...");
    refresh();
    getch();    
    endwin();   
}

void display_output(int fd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; 
        printw("%s", buffer);      
        refresh();                 
    }
}

void run_child_process(int pipe_fd[2], char *command) {
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

    execlp("/bin/sh", "sh", "-c", command, NULL);  
    perror("execlp failed");
    exit(1);
}

void get_user_command(char *cmd_buffer) {
    mvprintw(LINES - 1, 0, "Enter command: ");
    echo();
    getnstr(cmd_buffer, MAX_CMD_LEN -1);
    noecho();
    move(LINES -1, 0);
    clrtoeol();
}

