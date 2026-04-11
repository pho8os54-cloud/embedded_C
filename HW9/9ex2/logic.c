#include "logic.h"

void draw_files(WINDOW *win, const char *path, int selected, int active) // рисовка списка файлов в панели win - окно, path - текущая дирректория, selected - индекс выбранного файла, active - активна ли панель
{
    struct dirent **namelist;// получение размера окна для адаптации под размер окна
    int n; // кол-во файлов
    int max_y, max_x; // размеры
    getmaxyx(win, max_y, max_x);// размер окна(высоат, ширина)

    werase(win); //очистка окна
    // подсветка активной панели
    if (active) wattron(win, COLOR_PAIR(1) | A_BOLD);
    box(win, 0, 0);
    if (active) wattroff(win, COLOR_PAIR(1) | A_BOLD);

    mvwprintw(win, 0, 2, "[ %s ]", path);// отображение пути сверху

    n = scandir(path, &namelist, NULL, alphasort); // чтение файлов
    
    if (n >= 0) 
    {
        int height = max_y - 2; // высота области
        int offset = (selected >= height) ? selected - height + 1 : 0; // так называемый "скроллинг"

        for (int i = 0; i < n; i++) //цикл по файлу
        {
            if (i >= offset && (i - offset) < height) 
            {
                if (i == selected && active) wattron(win, A_REVERSE);//подсветка выбранного файла
                mvwprintw(win, i - offset + 1, 2, "%-.*s", max_x - 4, namelist[i]->d_name);//вывод имени
                if (i == selected && active) wattroff(win, A_REVERSE);//выкл подсветки
            }
            free(namelist[i]);//высвобождение
        }
        free(namelist);//высвобожение
    }
}

void view_file(const char *filename) //просмотр ф.
{
    FILE *f = fopen(filename, "r");//открытие файла
    if (!f) return;// если не открылся выходим

    // Проверка на бинарный файл 
    char check[4];
    if (fread(check, 1, 4, f) == 4 && check[0] == 0x7f && check[1] == 'E') //чтение первых 4 байтов
    {
        fclose(f);
        clear();
        mvaddstr(LINES/2, (COLS-25)/2, "За бинарники не шарю:(");
        refresh(); 
        getch(); 
        return;
    }
    rewind(f); //возврат в начало файла

    clear();
    char line[1024];
    int y = 0;
    while (fgets(line, sizeof(line), f) && y < LINES - 2)//чтение построчно
    {
        mvaddnstr(y++, 0, line, COLS);// вывод 
    }
    fclose(f);

    attron(A_REVERSE);
    mvprintw(LINES - 1, 0, " ПРОСМОТР: %s (Любая клавиша - выйти)", filename);
    attroff(A_REVERSE);
    refresh(); 
    getch(); 
    clear();
}