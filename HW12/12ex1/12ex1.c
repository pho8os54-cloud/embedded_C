/*Реализовать программу, которая создает неименованный канал и
порождает процесс. Родительский процесс закрывает дескриптор для
чтения из канала и записывает в канал строку "Hi!", затем ожидает
завершения дочернего процесса. Дочерний процесс закрывает
дескриптор для записи в канал, считывает строку из канала и выводит на
экран.*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main(void)
{
    int fildes[2];
    pid_t child_pid;
    char new_message[10];
    char message[]="Hi!";
    
    pipe(fildes); //запускаем канал
    
    child_pid = fork(); //создаем процесс
    
    if(child_pid > 0)
    {
        close(fildes[0]); // закрываем канал на чтение
        
        write(fildes[1], message, strlen(message)+1); // записываем в канал с дискриптором на запись
        
        close(fildes[1]); // закрываем канал на запись
        
        wait(NULL); //сьрасываем стек дочернего прочесса
    }
    else
    {
        close(fildes[1]); //закрываем канал на запись
        
        read(fildes[0], new_message, sizeof(new_message)); // читаем с канала с разрешенным дискритором на чтение
        
        printf("%s\n\n", new_message); 
        
        close(fildes[0]); // зыкрываем дискриптор на чтение
    }
    
    return 0;
}