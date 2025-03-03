#ifndef SCRIPT_VIEWER_H
#define SCRIPT_VIEWER_H

#include <ncurses.h>  // For window handling
#include <stdio.h>    // For file handling
#include <stdlib.h>   // For malloc/free
#include <string.h>   // For strdup
#include "tui.h"

#define MAX_LINES 1000   // Adjust based on needs
#define MAX_LINE_LENGTH 256  // Maximum length per line

void init_script_viewer(TuiState *state);
int load_script_file(TuiState *state, const char *filename);
void display_script(TuiState *state);
void scroll_script(TuiState *state, int direction);
void free_script_viewer(state);

#endif // SCRIPT_VIEWER_H
