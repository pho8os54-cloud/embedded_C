#include <stdio.h> // представление целого отрицательного числа в двоичной системе.
int main()
{
    int i;
    int Negative_number;
    printf("Введите целое отрицательное число: ");
    scanf("%d",&Negative_number);
    if (Negative_number<0)
    {
        printf("Двоичное представление числа %d:\n ",Negative_number);
        for ( i = 31; i >= 0; i--)
        {
            if (Negative_number&(1<<i))
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
        
        }
    printf("\n");
    }
        else
        {
            printf("Введите целое отрицательное число!!!\n");
        }
    
return 0; 
}