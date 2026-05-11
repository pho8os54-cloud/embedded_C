#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define FTOK_PATH "server.c" // макро для пути ключа
#define PROJECT_ID 67 // уникальный ключ
#define MAX_HISTORY 20 //история сообщений
#define MAX_USERS 50//количество пользователей
#define NAME_LEN 32//размер имени пользователя
#define TEXT_LEN 256//размер одного сообщения

struct message {
    char name[NAME_LEN];
    char text[TEXT_LEN];
};

struct chat_data {
    struct message history[MAX_HISTORY];//хрение последних 20 сообщений
    int history_count;
    char users[MAX_USERS][NAME_LEN];//хранение имен пользователей
    int user_count;//сколько в данный момент людей в чате
    int version; // Счетчик обновлений
};

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

#endif