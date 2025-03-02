#include "tui.h"

void init_ncurses() {
    initscr();
    start_color();
    use_default_colors();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
}

void init_panes(TuiState *state, int *screen_height, int *screen_width) {
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

    state->script_win = newwin(script_h, script_w, 0, 0);
    state->cmd_win = newwin(cmd_h, cmd_w, script_h, 0);
    state->output_win = newwin(out_h, out_w, script_h, cmd_w);
    state->vars_win = newwin(vars_h, vars_w, 0, script_w);
    state->watch_win = newwin(watch_h, watch_w, vars_h, script_w);
    state->stack_win = newwin(stack_h, stack_w, script_h, script_w);

    state->current_focus = FOCUS_INPUT;
}

void cleanup_ncurses(TuiState *state) {
    delwin(state->script_win);
    delwin(state->cmd_win);
    delwin(state->output_win);
    delwin(state->vars_win);
    delwin(state->watch_win);
    delwin(state->stack_win);
    endwin();}

void refresh_ui(TuiState *state) {
    clear();
    refresh();

    box(state->script_win, 0, 0);
    box(state->cmd_win, 0, 0);
    box(state->output_win, 0, 0);
    box(state->vars_win, 0, 0);
    box(state->watch_win, 0, 0);
    box(state->stack_win, 0, 0);

    highlight_focused_pane(state);

    wrefresh(state->script_win);
    wrefresh(state->cmd_win);
    wrefresh(state->output_win);
    wrefresh(state->vars_win);
    wrefresh(state->watch_win);
    wrefresh(state->stack_win);
    
}

void test_write(TuiState *state) {
    mvwprintw(state->script_win, 1, 1, "Script Viewer");
    mvwprintw(state->cmd_win, 1, 1, "Command Input");
    mvwprintw(state->output_win, 1, 1, "Command Output");
    mvwprintw(state->vars_win, 1, 1, "Variables");
    mvwprintw(state->watch_win, 1, 1, "Watch");
    mvwprintw(state->stack_win, 1, 1, "Call Stack");
}

void highlight_focused_pane(TuiState *state) {
    int  highlight_attr = A_BOLD | A_REVERSE;

    if (state->current_focus == FOCUS_SCRIPT) {
        wattron(state->script_win, highlight_attr);
	box(state->script_win, 0, 0);
	wattroff(state->script_win, highlight_attr);	
    } else {
        box(state->script_win, 0, 0);
    }
    if (state->current_focus == FOCUS_INPUT) {
        wattron(state->cmd_win, highlight_attr);
	box(state->cmd_win, 0, 0);
	wattroff(state->cmd_win, highlight_attr);
    } else {
        box(state->cmd_win, 0, 0);
    }
    if (state->current_focus == FOCUS_OUTPUT) {
        wattron(state->output_win, highlight_attr);
	box(state->output_win, 0, 0);
	wattroff(state->output_win, highlight_attr);
    } else {
        box(state->output_win, 0, 0);
    }
}

void switch_focus(TuiState *state) {
    if (state->current_focus == FOCUS_SCRIPT) {
        state->current_focus = FOCUS_INPUT; 
    } else if (state->current_focus == FOCUS_INPUT) {
        state->current_focus = FOCUS_OUTPUT;
    } else {
        state->current_focus = FOCUS_SCRIPT;
    }
}
