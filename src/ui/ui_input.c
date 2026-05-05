#include "ui_input.h"
#include <ncurses.h>
#include <string.h>

int ui_message_box(const char *title, const char *message, int height, int width) {
    int box_height = height;
    int box_width = width;
    int box_y = (LINES - box_height) / 2;
    int box_x = (COLS - box_width) / 2;

    WINDOW *win = newwin(box_height, box_width, box_y, box_x);
    if (!win) return 0;

    wbkgd(win, COLOR_PAIR(1));
    box(win, 0, 0);

    int title_len = strlen(title);
    mvwaddch(win, 0, box_width / 2 - title_len / 2 - 1, ACS_RTEE);
    waddstr(win, title);
    waddch(win, ACS_LTEE);

    int msg_y = 2;
    const char *ptr = message;
    while (*ptr && msg_y < box_height - 2) {
        char line[256] = {0};
        int line_len = 0;
        while (*ptr && line_len < box_width - 4) {
            line[line_len++] = *ptr++;
        }
        mvwaddstr(win, msg_y++, 2, line);
    }

    mvwaddstr(win, box_height - 2, box_width / 2 - 6, "[ENTER]");
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != KEY_ENTER && ch != '\n' && ch != 27) {
    }

    delwin(win);
    return 1;
}

int ui_get_input(const char *prompt, char *buffer, int max_len) {
    echo();
    curs_set(1);

    int y = LINES / 2 + 2;
    mvaddstr(y, 2, prompt);
    clrtoeol();
    refresh();

    getnstr(buffer, max_len - 1);

    noecho();
    curs_set(0);

    return strlen(buffer) > 0;
}