#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    const char *fifo = "myfile";
    int fildes;
    char buffer[10]; // Буфер для приема "Hi!"

    // Открываем только на чтение
    fildes = open(fifo, O_RDONLY);

    // Читаем данные
    read(fildes, buffer, sizeof(buffer));
    printf("Клиент получил: %s\n", buffer);

    close(fildes);

    // Удаляем канал
    unlink(fifo);

    return 0;
}