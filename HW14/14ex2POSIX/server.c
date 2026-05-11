/*-----------------POSIX---------------------------
Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
сегмент разделяемой памяти (достаточный для хранения сообщений) и
записывает сообщение виде строки “Hi!”, ждет ответа от клиента и
выводит на экран, удаляет сегмент разделяемой памяти. Клиент
подключается к сегменту разделяемой памяти и считывает сообщение от
сервера, выводит на экран, отвечает серверу сообщением виде строки
“Hello!”. */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define SPACE 1024

int main()
{
    int filder = shm_open("/server", O_CREAT | O_RDWR, 0666);
    ftruncate(filder, SPACE);

    char *ptr = mmap(NULL, SPACE, PROT_READ | PROT_WRITE, MAP_SHARED, filder, 0);

    // Создание именованного семафора (значение 0)
    sem_t *sem = sem_open("/server", O_CREAT, 0666, 0);

    strcpy(ptr, "Hi!");
    printf("Сервер записал: 'Hi!'. Ожидание клиента...\n");

    sem_wait(sem); 

    printf("Сервер получил ответ: %s\n", ptr);

    // Очистка ресурсов
    munmap(ptr, SPACE);
    close(filder);
    shm_unlink("/server"); // Удаление сегмента памяти
    sem_close(sem);
    sem_unlink("/server"); // Удаление семафора

    return 0;
}