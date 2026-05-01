/*Переписать командный интерпретатор из задания по процессам так,
чтобы он поддерживал конструкции вида:
$ ls -la | grep a*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    char input[256];
    char *args[256];
    int status;

    printf("Для выхода введите - exit \n\n");

    while(1)
    {
        printf("$ "); 
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) break;                     
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0) continue;

        int i = 0;
        args[i] = strtok(input, " \t");
        while (args[i] != NULL && i < 255) {
            args[++i] = strtok(NULL, " \t");
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0) break;

        int p_idx = -1;
        for (int j = 0; j < i; j++) {
            if (strcmp(args[j], "|") == 0) { p_idx = j; break; }
        }

        if (p_idx != -1) { // Если нашли конвейер
            int fd[2];
            pipe(fd);
            args[p_idx] = NULL; // Разделяем команды

            if (fork() == 0) { // Первый процесс (левый)
                dup2(fd[1], 1); 
                close(fd[0]); 
                close(fd[1]);
                execvp(args[0], args);
                exit(1);
            }
            if (fork() == 0) { // Второй процесс (правый)
                dup2(fd[0], 0); 
                close(fd[1]); 
                close(fd[0]);
                execvp(args[p_idx + 1], &args[p_idx + 1]);
                exit(1);
            }
            close(fd[0]); 
            close(fd[1]);
            wait(NULL); wait(NULL); // Ждем обоих
            continue; // Возврат к началу цикла
        }

        
        if (fork() == 0) {
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        }
        wait(&status);
    }
    return 0;
}