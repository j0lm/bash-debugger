#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

extern WINDOW *script_win, *cmd_win, *output_win, *vars_win, *watch_win, *stack_win;

void init_ncurses();
void init_panes(int *screen_height, int *screen_width);
void cleanup_ncurses();
void refresh_ui(int screen_height, int screen_width);
void test_write();
void color_bkg();

#endif
