/*Реализовать программу, которая порождает процесс. Родительский
процесс и дочерний выводят на экран свой pid, ppid. Затем родительский
ждет завершения дочернего и выводит его статус завершения.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>// для fork(), getpid(), getppid()
#include <sys/types.h> // для типа pid_t
#include <sys/wait.h> // для wait() и макросов WEXITSTATUS

int main(void)
{
    pid_t child_pid;
    int status;

    fflush(NULL);//сброс буффера, чтобы избежать дублирования 

    child_pid = fork();
    if (child_pid == 0)//дочерний процесс
    {
        printf("child_pid = %d\n", getpid());// возвращает pid текущего процесса
        exit(1);

    }
    else//родительский процесс
    {
        printf("Parent_pid = %d\n", getpid());
        wait(&status);//очищение стека дочернего процесса с возвращением статуса по процессу
        printf("Status = %d\n", WEXITSTATUS(status));
    }
exit(0);
}