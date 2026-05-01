/*Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
именованный канал и открывает его только на запись, записывает строку
“Hi!” и завершается. Клиент открывает созданный канал сервером
только на чтение, считывает строку из канала и выводит на экран. Удаляет
канал.*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>    //  для O_WRONLY
#include <string.h>   // для strlen

int main(void)
{

    const char *fifo= "myfile";
    int fildes;
    char message[]="Hi!";

    mkfifo(fifo, 0666);

    fildes = open(fifo, O_WRONLY);

    write(fildes, message, strlen(message)+1);

    close(fildes);

    return 0;
}

