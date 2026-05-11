#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>

void ui_init();
void ui_cleanup();
void ui_print_chat(const char* name, const char* text);
void ui_update_users(const char* name, int line_offset);
void ui_clear_users(); // Очистка окна онлайн
void ui_get_input(char* buffer, int max_len);

#endif