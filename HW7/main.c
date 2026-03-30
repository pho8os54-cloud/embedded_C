#include <stdio.h>
#include "calc.h"

int main()
{
    int choice;
    
    int  answer;

    int first_n, second_n;

while(1)
{
    printf("Введите первое число: ");
    scanf("%d", &first_n);

    printf("Введите второе число: ");
    scanf("%d", &second_n);

    printf("Выберите математическую операцию:\n1)сложение;\n2)вычитание;\n3)умножение;\n4)деление;\n5)выход;\nВведите номер операции: ");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
            printf("Результат операции сложения: %d\n", add(first_n, second_n));
            break;
        case 2:
            printf("Результат операции вычитание: %d\n", sub(first_n, second_n));
            break;
        case 3:
            printf("Результат операции умножение: %d\n", mul(first_n, second_n));
            break;
        case 4:
            printf("Результат операции деление: %f\n", div(first_n, second_n));
            break;
        case 5:
            printf("Выход\n");
            return 0;
        default:
         printf ("Выберите доступную операцию!\n");
         break;
        

    }
    
}
}