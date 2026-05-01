/*-----------------SYSTEM 5 ---------------------------
Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
очередь сообщений и записывает сообщение виде строки “Hi!”, ждет
ответа от клиента и выводит на экран, удаляет очередь. Клиент
подключается к очереди считывает сообщение от сервера, выводит на
экран, отвечает серверу сообщением виде строки “Hello!”*/ 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>

struct msgbuf
{
    long msg_type;
    char message[16];
};

int main()
{
    key_t key = ftok("13ex1server",67 );//создание ключа

    int queue = msgget(key, IPC_CREAT | 0666);//создание очереди

    struct msgbuf msg = { 1, "Hi!" };//сообщение
    
    msgsnd(queue, &msg, sizeof(msg.message), 0);//из программы в очередь

    printf("Сервер: отправлено '%s'\n", msg.message);

    msgrcv(queue, &msg, sizeof(msg.message), 2, 0);//из очереди в программу

    printf("Сервер: получен ответ '%s'\n", msg.message);

    msgctl(queue, IPC_RMID, NULL );//удаление очереди

    return 0;
}