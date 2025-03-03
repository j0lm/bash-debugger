#include "script_viewer.h"

void init_script_viewer(TuiState *state) {
    state->script_win_lines = malloc(MAX_LINES * sizeof(char *));
    if (!state->script_win_lines) {
        perror("failed to allocate memory for script viewer");
        exit(1);
    }
    state->script_win_line_count = 0;
    state->script_win_current_line = 0;
}

int load_script_file(TuiState *state, char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("error opening file");
	return -1;
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, file) && state->script_win_line_count < MAX_LINES) {
        state->script_win_lines[state->script_win_line_count] = strdup(buffer);
	if (!state->script_win_lines[state->script_win_line_count]) {
            perror("memory allocation failed");
	    fclose(file);
	    return -1;
	}
	state->script_win_line_count++;
    }
    fclose(file);
    return 0;
}

void display_script (TuiState *state) {
    werase(state->script_win);
    box(state->script_win, 0, 0);

    int max_x, max_y;
    getmaxyx(state->script_win, max_y, max_x);

    for (int i = 0; i < max_y -2 && i + state->script_win_current_line < state->script_win_line_count; i++) {
    mvwprintw(state->script_win, i + 1, 1, "%s", state->script_win_lines[state->script_win_current_line + i]);
    }

    wrefresh(state->script_win);
}

void scroll_script(TuiState *state, int direction) {
    if (direction == KEY_UP && state->script_win_current_line > 0) {
        state->script_win_current_line--;
    } else if (direction == KEY_DOWN && state->script_win_current_line < state->script_win_line_count - 1) {
        state->script_win_current_line++;
    }
    display_script(state);
}

void free_script_viewer(TuiState *state) {
    for (int i = 0; i < state->script_win_line_count; i++) {
        free(state->script_win_lines[i]);
    }
    free(state->script_win_lines);
}

