#ifndef LOGIC_H
#define LOGIC_H

#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

// Отрисовка списка файлов
void draw_files(WINDOW *win, const char *path, int selected, int active);

// Просмотр файла
void view_file(const char *filename);

#endif