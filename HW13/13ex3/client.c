#include <locale.h>//поддержка русского
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>//работа с потоками
#include <unistd.h>
#include "common.h"
#include "interface.h"

int msqid;//ключ потока
struct msgbuf my_data;//данные пользователя

void* receive(void* arg) {
    struct msgbuf incoming;
    while(1) {
        // sizeof(incoming) - sizeof(long) — это стандарт для System V
        if(msgrcv(msqid, &incoming, sizeof(incoming) - sizeof(long), getpid(), 0) > 0) {
            // обновление панели ОНЛАЙНА при входе 
            if (strstr(incoming.text, "присоединился") || strstr(incoming.text, "уже в чате")) {
                ui_update_users(incoming.name);
            }
            ui_print_chat(incoming.name, incoming.text);//вывод сообщения в окно чата
        }
    }
    return NULL;
}

int main() {
    setlocale(LC_ALL, "");//поддержка русского

    key_t key = ftok(FTOK_PATH, PROJECT_ID);//генераация ключа
    msqid = msgget(key, 0666);//подключаемся к очереди
    if (msqid == -1) {
        printf("Ошибка: Очередь не найдена. Сначала запусти сервер!\n");
        exit(1);
    }
    
    //регистрация пользователя
    printf("Ваше имя: ");
    scanf("%31s", my_data.name);
    getchar(); // Очистка буфера от \n
    my_data.sender_pid = getpid();//запоминаем pid

    ui_init();
    
    //Добавляем себя в список онлайн 
    ui_update_users(my_data.name);

    pthread_t thread;//создаем второй поток
    pthread_create(&thread, NULL, receive, NULL);

    // Сигнал сервера с 1 типом
    my_data.mtype = 1;
    strcpy(my_data.text, "присоединился к чату");
    msgsnd(msqid, &my_data, sizeof(my_data) - sizeof(long), 0);

    while(1) //ожидание ввода пользователя
    {
        ui_get_input(my_data.text, 255);
        if(strlen(my_data.text) > 0) {
            my_data.mtype = 1;
            msgsnd(msqid, &my_data, sizeof(my_data) - sizeof(long), 0);
            // Свое сообщение выводим локально
            ui_print_chat("Вы", my_data.text);
        }
    }

    ui_cleanup();
    return 0;
}