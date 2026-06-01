/*Написать программу, в которой реализуется event-loop с помощью
вызова sigwait(). При запуске программа блокирует сигнал SIGUSR1 и
переходит в бесконечный цикл ожидания сигнала, при получении сигнала
на экран выводится сообщение о получении сигнала и осуществляется
переход на следующую итерацию цикла. Проверить программу с помощью
второй программы из задания 1 и утилиты kill*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    sigset_t set; // Набор сигналов
    int received_sig; // Переменная для записи номера полученного сигнала

    //Инициализируем набор и добавляем в него SIGUSR1
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    //Блокируем SIGUSR1 для sigwait
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("PID: %d\n", getpid());

    //event-loop
    while (1) {
        // sigwait блокирует выполнение программы, пока не придет сигнал из набора set
        // Когда сигнал приходит, функция записывает его номер в received_sig
        sigwait(&set, &received_sig);

        
        if (received_sig == SIGUSR1) {
            printf("Сигнал SIGUSR1 получен\n");
        }
    }

    return 0;
}