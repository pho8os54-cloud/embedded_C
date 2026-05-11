#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define SPACE 1024

int main()
{
    int filder = shm_open("/server", O_RDWR, 0666);

    char *ptr = mmap(NULL, SPACE, PROT_READ | PROT_WRITE, MAP_SHARED, filder, 0);

    //Открытие существующего семафора
    sem_t *sem = sem_open("/server", 0);

    printf("Клиент: прочитал от сервера: %s\n", ptr);

    strcpy(ptr, "Hello!");
    printf("Клиент отправил ответ: Hello!\n");

    sem_post(sem);

    munmap(ptr, SPACE);
    close(filder);
    sem_close(sem);

    return 0;
}