/*Реализовать аналог командного интерпретатора bash. При запуске
программы пользователю предлагается ввести имя программы и опции
запуска программы. Программа порождает процесс и в нем выполняет
введенную программу с заданными опциями, ждет завершения
дочернего процесса. Снова возвращается к вводу следующей
программы. Выход из интерпретатора по команде exit.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>// для fork(), getpid(), getppid(),execvp()
#include <sys/types.h> // для типа pid_t
#include <sys/wait.h> // для wait() и макросов WEXITSTATUS
#include <string.h>//сревнение строк

int main(void)
{
    char input[256];// строка ввода
    char *args[256];// массив аргументов
    pid_t child_pid;
    int status;

    printf("Для выхода введите - exit \n\n");

    while(1)
    {
        fflush(NULL);

        // Читаем всю строку целиком c с пробелами
        if (fgets(input, sizeof(input), stdin) == NULL) 
        {
            break;                     
        }

        int i = 0; // счетчик для заполнения массива аргументов
        // Убираем символ новой строки
        input[strcspn(input, "\n")] = '\0';

        // Если строка пустая — продолжаем
        if (strlen(input) == 0) // при вводе пустой строки возвращаемся к началу цикла
        {
            continue;
        }

        // Разбиваем строку на аргументы
       
        args[i] = strtok(input, " \t");// разбивка cтроки на слова и первое слово кладем в массив

        while (args[i] != NULL && i < 256) //продолжение разбивки на слова и запись в массив
        {
            args[++i] = strtok(NULL, " \t");
        }
        args[i] = NULL;// в конец массива

        // Выход по команде exit
        if (strcmp(args[0], "exit") == 0) {
            printf("Выход\n");
            break;
        }

        // Создаём дочерний процесс
        child_pid = fork();

        if (child_pid == 0)        // дочерний процесс
        {
            execvp(args[0], args);
            perror("execvp");      // если команда не найдена
            exit(1);
        }
        else if (child_pid > 0)    // родительский процесс
        {
            wait(&status);
        }
        else
        {
            perror("fork");        // ошибки при создании процесса
        }
    }

    return(0);
}