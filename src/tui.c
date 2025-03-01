#include "tui.h"

WINDOW *script_win, *cmd_win, *output_win, *vars_win, *watch_win, *stack_win;

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
}

void init_panes(int *screen_height, int *screen_width) {
    getmaxyx(stdscr, *screen_height, *screen_width);
    
    int script_h = (2 * *screen_height) / 3;
    int script_w = (2 * *screen_width) / 3;
    int cmd_h = *screen_height - script_h;
    int cmd_w = script_w / 2;
    int out_h = cmd_h;
    int out_w = script_w - cmd_w;
    int vars_h = script_h / 2;
    int vars_w = *screen_width - script_w;
    int watch_h = script_h - vars_h;
    int watch_w = vars_w;
    int stack_h = *screen_height - script_h;
    int stack_w = vars_w;

    script_win = newwin(script_h, script_w, 0, 0);
    cmd_win = newwin(cmd_h, cmd_w, script_h, 0);
    output_win = newwin(out_h, out_w, script_h, cmd_w);
    vars_win = newwin(vars_h, vars_w, 0, script_w);
    watch_win = newwin(watch_h, watch_w, vars_h, script_w);
    stack_win = newwin(stack_h, stack_w, script_h, script_w);
}

void cleanup_ncurses() {
    delwin(script_win);
    delwin(cmd_win);
    delwin(output_win);
    delwin(vars_win);
    delwin(watch_win);
    delwin(stack_win);
    endwin();
}

void refresh_ui() {
    clear();
    refresh();

    box(script_win, 0, 0);
    box(cmd_win, 0, 0);
    box(output_win, 0, 0);
    box(vars_win, 0, 0);
    box(watch_win, 0, 0);
    box(stack_win, 0, 0);

    wrefresh(script_win);
    wrefresh(cmd_win);
    wrefresh(output_win);
    wrefresh(vars_win);
    wrefresh(watch_win);
    wrefresh(stack_win);
    
}

void test_write() {
    mvwprintw(script_win, 1, 1, "Script Viewer");
    mvwprintw(cmd_win, 1, 1, "Command Input");
    mvwprintw(output_win, 1, 1, "Command Output");
    mvwprintw(vars_win, 1, 1, "Variables");
    mvwprintw(watch_win, 1, 1, "Watch");
    mvwprintw(stack_win, 1, 1, "Call Stack");
}
