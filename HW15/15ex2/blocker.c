/*Реализовать программу, которая будет блокировать сигнал SIGINT с
помощью вызова sigprocmask() и уходить в бесконечный цикл. Проверить
работу программы с помощью второй программы из задания 1 и утилиты
kill*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    sigset_t set;

    // Подготовка набора сигналов
    sigemptyset(&set);          // Очищаем набор
    sigaddset(&set, SIGINT);    // Добавляем в набор сигнал SIGINT (Ctrl+C)

    // Блокируем сигналы из набора
    // SIG_BLOCK добавляет SIGINT к уже существующим маскам процесса
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("SIGINT (Ctrl+C) заблокирован. Мой PID: %d\n", getpid());
    printf("Попробуйте нажать Ctrl+C или отправить сигнал kill\n");

    // Бесконечный цикл ожидания
    while(1) {
        sleep(1);
    }

    return 0;
}