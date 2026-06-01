#include <stdlib.h> // Для функции atoi преобразование строки в число
#include <signal.h> // Для системного вызова kill и константы SIGUSR1

int main(int argc, char *argv[]) {
    // argv[1] — это PID, который мы передаем при запуске программы в терминале
    // atoi превращает текстовый PID в целое число
    int target_pid = atoi(argv[1]);

    // Системный вызов kill отправляет сигнал указанному процессу
    kill(target_pid, SIGUSR1);

    return 0;
}