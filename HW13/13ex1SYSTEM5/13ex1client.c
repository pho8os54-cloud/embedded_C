#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>

struct msgbuf
{
    long msg_type;
    char message[16];
};

int main()
{
    key_t key = ftok("13ex1server", 67); //создание инд. ключа

    int queue = msgget(key, 0666);//подключение к очередеи
    
    struct msgbuf msg;

    msgrcv(queue, &msg, sizeof(msg.message), 1, 0);// из очереди в программу
    printf("Клиент: получено '%s'\n", msg.message);
    
    msg.msg_type = 2; 
    strcpy(msg.message, "Hello!");
    
    msgsnd(queue, &msg, sizeof(msg.message), 0);// из программы в очередь
    printf("Клиент: отправлен ответ '%s'\n", msg.message);

    return 0;
}