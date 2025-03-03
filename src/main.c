#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tui.h"
#include "script_viewer.h"

extern char* strdup(const char*);
char *get_script(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    const char *filename = strdup(argv[1]);
    // incorrect usage output
    if (argc <  2) {
        printf("Usage: %s <script_path [script_args...]\n", argv[0]);
	return 1;
    }
    char *script_string = get_script(argc, argv);
    printf("Script to run: %s\n", script_string);

    int screen_height, screen_width;
    TuiState state;

    // init
    init_ncurses();
    init_panes(&state, &screen_height, &screen_width);
    refresh_ui(&state);
    init_script_viewer(&state);
    if (load_script_file(&state, filename) == -1) {
        return -1;
    }
    
    // input loop
    int ch;
    while((ch = getch()) != 'q') {
       if (ch == '\t') {
           switch_focus(&state);
       } if (ch == KEY_UP || ch == KEY_DOWN) {
           scroll_script(&state, ch); 
       } 
    
       refresh_ui(&state);
    }

    getch();
    cleanup_ncurses(&state);
    free(script_string);
    return 0;
}

char *get_script(int argc, char *argv[]) {
    // calculate length
    int total_length = 0;
    for (int i = 1; i < argc; i++) {
        total_length += strlen(argv[i]) + 3; // space + 2 quotes
    }
    
    // allocate memory
    char *command = malloc(total_length);
    if (!command) {
        perror("malloc failed");
        return NULL;
    }

    // initialize empty string
    command[0] = '\0';
    
    // combine arguments
    snprintf(command, total_length + 1, "\"%s\"", argv[1]);
    for (int i = 2; i < argc; i++) {
        strcat(command, " \"");
        strcat(command, argv[i]);
        strcat(command, "\"");
    } 

    return command;
}
