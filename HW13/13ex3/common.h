#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <sys/ipc.h> // Ф для IPC
#include <sys/msg.h> //Ф   для очередей

#define FTOK_PATH "server.c" //путь к файлу
#define PROJECT_ID 67//уникальный ключ

struct msgbuf //структура сообщения
{
    long mtype;
    int  sender_pid;//PID отправителя
    char name[32]; //имя пользователя
    char text[256];//сообщение
};

#endif