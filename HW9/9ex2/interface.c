#include <ncurses.h>
#include <locale.h> // отображение русского
#include "logic.h"

int main() {
    setlocale(LC_ALL, ""); // для отображения русского
    initscr(); //переход в режим работы с интерфейсом
    cbreak(); // отключение буфферизации - чтобы сразу отображать нажатие 
    noecho(); // выкл. отоброжения набранных символов
    keypad(stdscr, TRUE); //работа с клавишами
    curs_set(0); // скрыть курсор
    start_color(); // вкл поддержка цветов 
    init_pair(1, COLOR_GREEN, COLOR_BLACK); //инициализация цвета

    char path1[1024], path2[1024]; //левая и правая панель 
    getcwd(path1, 1024); getcwd(path2, 1024); // массивы для хренения пути
    int sel1 = 0, sel2 = 0; //индексы выбранного файла
    int active_panel = 0; // какая панель активна: 0 - левая, 1 - правая

    while (1) 
    {
        int w = COLS / 2; // делаем 2 панели 
        // LINES - 1 оставляет место в самом низу для строки статуса
        WINDOW *win1 = newwin(LINES - 1, w, 0, 0); // 1 панель Высота 1, ширина w, позиция 0 0 
        WINDOW *win2 = newwin(LINES - 1, COLS - w, 0, w); // 1 панель Высота 1, ширина w, позиция 0 w
        keypad(win1, TRUE); keypad(win2, TRUE);// включение кнопок для панели

        draw_files(win1, path1, sel1, active_panel == 0); // список файлов
        draw_files(win2, path2, sel2, active_panel == 1);

        
        attron(A_REVERSE);// выделение - инверсия фона  и текста нижнего табла
        mvprintw(LINES - 1, 0, " q: Выход | TAB: Сменить панель | ENTER: Открыть | Стрелки: Навигация "); //строка внизу 
        // Дозаполняем строку до конца экрана
        for(int i = getcurx(stdscr); i < COLS; i++) addch(' ');// заполнения низа строки пробелами
        attroff(A_REVERSE); // выключение инверсии

        refresh(); wrefresh(win1); wrefresh(win2); // обновление экранов и вывод

        //ВВод
        int ch = getch(); 
        if (ch == 'q' || ch == 'Q') break; // Выход "Q" (обязательно ра анг расскладке)
        if (ch == '\t') active_panel = !active_panel; // переключение панели
        
        //работа с активной панелью
        int *curr_sel = (active_panel == 0) ? &sel1 : &sel2; 
        char *curr_path = (active_panel == 0) ? path1 : path2;
        

        //чтение файлов 
        struct dirent **list;
        int n = scandir(curr_path, &list, NULL, alphasort);

        //Навигация
        if (ch == KEY_UP && *curr_sel > 0) (*curr_sel)--;//Вверх
        if (ch == KEY_DOWN && n > 0 && *curr_sel < n - 1) (*curr_sel)++;//вниз
        
        if (ch == 10 && n > 0) // нажатие enter и присутствие файлов в папке
        { 
            char next[2048]; //массив для пути к выбранному объекту
            sprintf(next, "%s/%s", curr_path, list[*curr_sel]->d_name);// запись строки в массив, curr_path - текущая папка
            struct stat st; // получение инфы по файлу
            if (stat(next, &st) == 0) //проверка типа и в зависимости от него открыть или перейти
            {
                if (S_ISDIR(st.st_mode)) //проверка на директорию
                {
                    if (strcmp(list[*curr_sel]->d_name, ".") != 0) 
                    {
                        chdir(next);
                        getcwd(curr_path, 1024); // получение текущей дирректории и запись в структуру
                        *curr_sel = 0; //сброс выбора на первый элемент в новой диреткории
                        clear(); //очистка экрана
                    }
                } 
                else 
                {
                    view_file(next);
                }
            }
        }

        if (n > 0) // очистка памяти
        {
            for(int i = 0; i < n; i++) free(list[i]);
            free(list);
        }

        if (ch == KEY_RESIZE) clear(); //динамическое изменение окна менеджера и clear очистка, чтобы не было наложенний 
        delwin(win1); 
        delwin(win2);
    }

    endwin();
    return 0;
}