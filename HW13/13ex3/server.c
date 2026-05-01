#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define MAX_HISTORY 20 // сколько сообщений храним для передачи новому пользователю
#define MAX_USERS 50 // максимальное количество пользователей

struct msgbuf history[MAX_HISTORY]; //массив структур сообщений
int history_count = 0; // счетчик сообщений в истории

struct {
    int pid;
    char name[32];
} 
active_users[MAX_USERS];//список онлайна
int user_count = 0;//счетчик количества пользователей

void add_to_history(struct msgbuf m)//добавление сообщений в интерфейс новому пользоввателю
{
    if (history_count < MAX_HISTORY) {
        history[history_count++] = m;
    } else {
        for (int i = 0; i < MAX_HISTORY - 1; i++) history[i] = history[i+1];
        history[MAX_HISTORY - 1] = m;
    }
}

int main() {
    key_t key = ftok(FTOK_PATH, PROJECT_ID);//создание ключа
    int msqid = msgget(key, 0666 | IPC_CREAT);//создаем или подключаемся к очереди
    struct msgbuf msg;

    printf("сервер запущен...\n");

    while(1) {
        // Читаем сообщения ТОЛЬКО с типом 1 (предназначенные серверу)
        if (msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1, 0) == -1) continue;

        int user_idx = -1;
        for(int i = 0; i < user_count; i++) {
            if(active_users[i].pid == msg.sender_pid) user_idx = i;
        }
        
        // Если пользователь новый
        if(user_idx == -1 && user_count < MAX_USERS) {
            // Отправляем новичку историю последних сообщений
            for(int i = 0; i < history_count; i++) {
                struct msgbuf old = history[i];
                old.mtype = msg.sender_pid;
                msgsnd(msqid, &old, sizeof(old) - sizeof(long), 0);
            }
            //Отправляем новичку тех, кто уже в сети
            for(int i = 0; i < user_count; i++) {
                struct msgbuf notify = {.mtype = msg.sender_pid, .sender_pid = active_users[i].pid};
                strcpy(notify.name, active_users[i].name);
                strcpy(notify.text, "уже в чате");
                msgsnd(msqid, &notify, sizeof(notify) - sizeof(long), 0);
            }
            // Сохраняем нового пользователя
            active_users[user_count].pid = msg.sender_pid;
            strcpy(active_users[user_count].name, msg.name);
            user_count++;
        }

        // Сохраняем в историю (если это не служебные сообщения)
        if (strstr(msg.text, "присоединился") == NULL) {
            add_to_history(msg);
        }

        // Рассылаем всем остальным
        for (int i = 0; i < user_count; i++) {
            if (active_users[i].pid != msg.sender_pid)//себе не шлем
            {
                msg.mtype = active_users[i].pid;
                msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);
            }
        }
    }
    return 0;
}