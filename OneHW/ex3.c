/*Найти количество единиц в двоичном представлении
целого положительного числа (число вводится с клавиатуры).*/
#include <stdio.h>
int main()
{
    
    int P_number;
    printf("Введите целое положительное число: ");
    scanf("%d",&P_number);
    
    if (P_number>0)
    {
        int i;
        int counter=0;
        printf("Введеное число: %d\n",P_number);
        for ( i = 31; i >=0 ; i--)
        {
            if(P_number&(1<<i))
                {
                    counter++;
                
                }
        
        }
        printf("Количество единиц в двичном представлении: %d\n", counter);
    }
    else
    {
        printf("Введите положительное число!!!\n");
    }
return 0;
    
}