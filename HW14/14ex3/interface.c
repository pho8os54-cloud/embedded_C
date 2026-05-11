#include "interface.h"
#include <string.h>

static WINDOW *win_chat, *win_users, *win_input;

void ui_init() {
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(1);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    win_chat = newwin(max_y - 3, max_x - 30, 0, 0);
    win_users = newwin(max_y - 3, 30, 0, max_x - 30);
    win_input = newwin(3, max_x, max_y - 3, 0);

    scrollok(win_chat, TRUE);

    wbkgd(win_chat, COLOR_PAIR(1));
    wbkgd(win_users, COLOR_PAIR(1));
    wbkgd(win_input, COLOR_PAIR(1));

    ui_clear_users();
    box(win_chat, 0, 0);
    box(win_input, 0, 0);
    
    refresh();
    wrefresh(win_chat);
    wrefresh(win_input);
}

void ui_clear_users() {
    werase(win_users);
    box(win_users, 0, 0);
    wattron(win_users, A_BOLD);
    mvwprintw(win_users, 1, 1, "ОНЛАЙН:");
    wattroff(win_users, A_BOLD);
    wrefresh(win_users);
}

void ui_print_chat(const char* name, const char* text) {
    wattron(win_chat, A_BOLD);
    wprintw(win_chat, "\n %s: ", name);
    wattroff(win_chat, A_BOLD);
    wprintw(win_chat, "%s", text);
    box(win_chat, 0, 0);
    wrefresh(win_chat);
}

void ui_update_users(const char* name, int line_offset) {
    wattron(win_users, A_BOLD);
    mvwprintw(win_users, 2 + line_offset, 1, "• %s", name);
    wattroff(win_users, A_BOLD);
    wrefresh(win_users);
}

void ui_get_input(char* buffer, int max_len) {
    werase(win_input);
    box(win_input, 0, 0);
    mvwprintw(win_input, 1, 1, "> ");
    echo();
    wgetnstr(win_input, buffer, max_len);
    noecho();
}

void ui_cleanup() {
    endwin();
}