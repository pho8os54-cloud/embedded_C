#include "interface.h"
#include <string.h>

static WINDOW *win_chat, *win_users, *win_input;//3 окна

void ui_init() {
    initscr();// Инициализация ncurses
    start_color();// Включение цветового режима
    cbreak();// Немедленный ввод (без Enter для спецклавиш)
    noecho();//Не печатать вводимые символы автоматически
    curs_set(1);// Показать курсор

    // Создаем цветовую пару
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);// Узнаем размер окна терминала

    // Создаем окна
    win_chat = newwin(max_y - 3, max_x - 30, 0, 0);
    win_users = newwin(max_y - 3, 30, 0, max_x - 30);
    win_input = newwin(3, max_x, max_y - 3, 0);

    // Включаем прокрутку для чата
    scrollok(win_chat, TRUE);

    // Применяем цветовую пару ко всем окнам и фону
    wbkgd(win_chat, COLOR_PAIR(1));
    wbkgd(win_users, COLOR_PAIR(1));
    wbkgd(win_input, COLOR_PAIR(1));

    // Рисуем рамки
    box(win_chat, 0, 0);
    box(win_users, 0, 0);
    box(win_input, 0, 0);

    mvwprintw(win_users, 1, 1, "ОНЛАЙН:");//заголовок в правом окне

    refresh();//обновление
    wrefresh(win_chat);
    wrefresh(win_users);
    wrefresh(win_input);
}

void ui_print_chat(const char* name, const char* text) {
    // жирный шрифт
    wattron(win_chat, A_BOLD);
    wprintw(win_chat, "\n %s: ", name);//печать имени
    wattroff(win_chat, A_BOLD);//выкл жирного шрифта
    
    wprintw(win_chat, "%s", text);//печать сообщения
    
    box(win_chat, 0, 0); //на всякий случай восстанавливаем рамку
    wrefresh(win_chat); //обновление
}

void ui_update_users(const char* name) {
    static int line = 2;
    wattron(win_users, A_BOLD);
    mvwprintw(win_users, line++, 1, "• %s", name);
    wattroff(win_users, A_BOLD);
    
    box(win_users, 0, 0);
    wrefresh(win_users);
}

void ui_get_input(char* buffer, int max_len) {
    werase(win_input);
    box(win_input, 0, 0);
    wattron(win_input, A_BOLD);
    mvwprintw(win_input, 1, 1, "> ");
    wattroff(win_input, A_BOLD);
    
    echo();

    wgetnstr(win_input, buffer, max_len);
    noecho();
}

void ui_cleanup() {
    endwin();
}