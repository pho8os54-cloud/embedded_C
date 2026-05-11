/*-----------------SYSTEM 5 ---------------------------
Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
сегмент разделяемой памяти (достаточный для хранения сообщений) и
записывает сообщение виде строки “Hi!”, ждет ответа от клиента и
выводит на экран, удаляет сегмент разделяемой памяти. Клиент
подключается к сегменту разделяемой памяти и считывает сообщение от
сервера, выводит на экран, отвечает серверу сообщением виде строки
“Hello!”. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/sem.h>

#define SPACE 1024


int main()
{
    key_t key = ftok("server",67 );//создание ключа
    int sh_memory = shmget(key, SPACE, IPC_CREAT | 0666);
    char *list = (char *)shmat(sh_memory, NULL, 0);

    int semid = semget(key, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 0);

    strcpy(list, "Hi!");
    printf("Сервер записал: %s. Ожидание клиента...\n", list);

    //ждем, пока семафор станет больше 0
    struct sembuf wait_client = {0, -1, 0}; 
    semop(semid, &wait_client, 1);

    printf("Сервер получил ответ: %s\n", list);

    shmdt(list);
    shmctl(sh_memory, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID); // Удаляем семафор
    return 0;
}