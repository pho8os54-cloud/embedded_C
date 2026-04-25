/*Реализовать программу, которая порождает процесс1 и процесс2, ждет
завершения дочерних процессов. Процесс1 в свою очередь порождает
процесс3 и процесс4 и ждет их завершения. Процесс2 порождает
процесс5 и ждет его завершения. Все процессы выводят на экран свой
pid, ppid.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>// для fork(), getpid(), getppid()
#include <sys/types.h> // для типа pid_t
#include <sys/wait.h> // для wait() и макросов WEXITSTATUS

int main(void)
{
    pid_t child_pid, child_pid2, child_pid3, child_pid4, child_pid5;
    int status;

    fflush(NULL);//сброс буффера, чтобы избежать дублирования 

    child_pid = fork(); // создание 1 процесса
    if (child_pid == 0)
    {
        printf("child_pid=%d\n parent_pid=%d\n\n", getpid(), getppid());
        
        child_pid3 = fork();// созд. 3 проц.
        if (child_pid3 == 0)
            {
                printf("child_pid3=%d\n parent_pid=%d\n\n", getpid(), getppid());
                exit(1);
            }
            else
                {
                    wait(&status);//очищение стека 3 п.
                }
        
        child_pid4 = fork();//созд. 4 проц. 
        if  (child_pid4 == 0)
            {
                printf("child_pid4=%d\n parent_pid=%d\n\n", getpid(), getppid());
                exit(1);
            }
            else
                {
                    wait(&status);//очищ. стека 4 п.
                }
      
    }
        else
        {
            child_pid2 = fork();//созд. 2 проц.
            if (child_pid2 == 0)
                {
                    printf("child_pid2=%d\n parent_pid=%d\n\n", getpid(), getppid());
                    
                    child_pid5 = fork();//созд. 5 проц.
                    if (child_pid5 == 0)
                        {
                            printf("child_pid5=%d\n parent_pid=%d\n\n", getpid(), getppid());
                            exit(1);
                        }
                        else
                        {
                            wait(&status);//очищение стека 5 проц.
                            exit(1);
                        }
                }
            else
                {
                    // чищение стеков 1 и 2 процессов.
                    wait(&status);
                    wait(&status);
                    exit(1);
                }    
               
        }
    
exit(1);
}