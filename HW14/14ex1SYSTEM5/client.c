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
    key_t key = ftok("server",67 );
    int sh_memory = shmget(key, SPACE, 0666);
    char *list = (char*)shmat(sh_memory,NULL,0);

    // Подключаемся к семафору
    int semid = semget(key, 1, 0666);

    printf("Клиент прочитал: %s\n", list);

    strcpy(list, "Hello!");
    printf("Клиент отправил ответ: Hello!\n");

    // Операция: увеличиваем семафор на 1 (сигнал серверу)
    struct sembuf signal_server = {0, 1, 0};
    semop(semid, &signal_server, 1);
    
    shmdt(list);
    return 0;
}