#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

int main() {
    key_t key = ftok(FTOK_PATH, PROJECT_ID);//генерируем уник ключ
    
    int shmid = shmget(key, sizeof(struct chat_data), IPC_CREAT | 0666);//создаем раздел память
    struct chat_data *chat = (struct chat_data *)shmat(shmid, NULL, 0);//отображение страниц в наше адресное про-во
    memset(chat, 0, sizeof(struct chat_data));//очищаем память перед использованием

    int semid = semget(key, 1, IPC_CREAT | 0666);//создание семафора
    union semun arg;
    arg.val = 1; //начальное значение семафора
    semctl(semid, 0, SETVAL, arg);
    /*SETVAL используется для непосредственной установки значения конкретного семафора в наборе.
    Управляющая команда.Устанавливает значение семафора равным arg.val*/

    printf("Сервер запущен. Ожидание клиентов...\n");

    while(1) sleep(10);

    return 0;
}