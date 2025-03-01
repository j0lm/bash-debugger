#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

extern WINDOW *script_win, *cmd_win, *output_win, *vars_win, *watch_win, *stack_win;

void init_ncurses();
void init_panes(int *screen_height, int *screen_width);
void cleanup_ncurses();
void refresh_ui();
void test_write();

#endif
