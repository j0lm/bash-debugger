#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

extern WINDOW *script_win, *cmd_win, *output_win, *vars_win, *watch_win, *stack_win;

typedef enum {
    FOCUS_SCRIPT,
    FOCUS_INPUT,
    FOCUS_OUTPUT
} FocusPane;

typedef struct {
    WINDOW *script_win;
    WINDOW *cmd_win;
    WINDOW *output_win;
    WINDOW *vars_win;
    WINDOW *watch_win;
    WINDOW *stack_win;
    FocusPane current_focus;
    char **script_win_lines;
    int script_win_line_count;
    int script_win_current_line;
} TuiState;

void init_ncurses();
void init_panes(TuiState *state, int *screen_height, int *screen_width);
void cleanup_ncurses(TuiState *state);
void refresh_ui(TuiState *state);
void test_write(TuiState *state);
void highlight_focused_pane(TuiState *state);
void switch_focus(TuiState *state);

#endif
