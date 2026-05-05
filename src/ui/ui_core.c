#include "ui_core.h"
#include <ncurses.h>

void ui_init(void) {
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_CYAN, COLOR_BLUE);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void ui_cleanup(void) {
    endwin();
}