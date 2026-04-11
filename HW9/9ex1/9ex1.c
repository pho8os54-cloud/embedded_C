#include <stdio.h>
#include <fcntl.h> //флаги
#include <unistd.h> //системные вызовы(write и т.д.)

int main()
{
    int fd = open("output.txt", O_CREAT | O_RDWR, 0644);//создание/открытие файла, права доступа
    char kit[] = "String from file";

    //вычисление длины строки
    int len = 0;
    while (kit[len] != '\0')
    {
        len++;
    }
    write(fd, kit, len);//запись в файл 
    char ch;

    for(int i = len - 1; i >= 0; i --)//вывод с конца
    {
        lseek(fd, i, SEEK_SET);
        read(fd, &ch, 1);// чтение побайтно 
        printf("%c", ch);
    }

    printf("\n");

    close(fd);//закрытие файла

    return 0;
}
