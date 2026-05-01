#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>

// Инициализация и закрытие
void ui_init();
void ui_cleanup();

// Функции для работы с окнами
void ui_print_chat(const char* name, const char* text);
void ui_update_users(const char* names_list);
void ui_get_input(char* buffer, int max_len);

#endif