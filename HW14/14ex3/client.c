#include <locale.h>//поддержка русского
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "interface.h"

struct chat_data *chat;//Указатель, который будет хранить адрес начала разделяемой памяти
int semid;//хранение индифик. семафора
char my_name[NAME_LEN];//имя текущего пользователя
int last_version = 0;

//реализация так называемого мьютекса на семафорах
void lock() {
    struct sembuf op = {0, -1, 0};
    semop(semid, &op, 1);
}

void unlock() {
    struct sembuf op = {0, 1, 0};
    semop(semid, &op, 1);
}


void* receive_thread(void* arg)//появление сообщений  
{
    while(1) {
        if (chat->version > last_version) //если версия в памяти больше, значит, в чате есть что-то новое
        {
            lock();//заход в крит секцию
            
            // Новые сообщения, вывод из нашего буфера
            for (int i = last_version; i < chat->version; i++) {
                int idx = i % MAX_HISTORY;
                if (strcmp(chat->history[idx].name, my_name) != 0) {
                    ui_print_chat(chat->history[idx].name, chat->history[idx].text);
                }
            }
            
            // Обновление списка онлайн 
            ui_clear_users();
            for (int i = 0; i < chat->user_count; i++) {
                ui_update_users(chat->users[i], i);
            }
            
            last_version = chat->version;
            unlock();//выход из крит секции
        }
        
    }
    return NULL;
}

int main() {
    setlocale(LC_ALL, "");//поддержка русского
    key_t key = ftok(FTOK_PATH, PROJECT_ID);//создаем ключ
    printf("Ваше имя: ");
    scanf("%31s", my_name);
    getchar();

    int shmid = shmget(key, sizeof(struct chat_data), 0666);
    chat = (struct chat_data *)shmat(shmid, NULL, 0);
    semid = semget(key, 1, 0666);// Доступ к семафору

    ui_init();//запуск интерфейса

    lock();
    if (chat->user_count < MAX_USERS) {
        strcpy(chat->users[chat->user_count++], my_name);
        int idx = chat->version % MAX_HISTORY;
        strcpy(chat->history[idx].name, "SYSTEM");
        sprintf(chat->history[idx].text, "%s зашел в чат", my_name);
        chat->version++;
    }
    unlock();
// Запуск второго потока для прослушивания
    pthread_t thread;
    pthread_create(&thread, NULL, receive_thread, NULL);

    char input[TEXT_LEN];
    while(1) {
        ui_get_input(input, TEXT_LEN - 1);
        if (strlen(input) > 0) {
            lock();
            int idx = chat->version % MAX_HISTORY;
            strcpy(chat->history[idx].name, my_name);
            strcpy(chat->history[idx].text, input);
            chat->version++;
            unlock();
            ui_print_chat("Вы", input);
        }
    }

    ui_cleanup();
    return 0;
}