/*Вывести заданный массив размером N
в обратном порядке.*/

#include <stdio.h>
#define N 27

int main()
{
    char i;
    char remember;
    char massiv[N];
   
for(i=0;i<N;i++)
    {
        
        massiv[i]=i+1;
        printf("%d ",massiv[i]);
        
    }

for(i=0;i<N/2;i++) // замена элементов
    {
        remember = massiv[i];
        massiv[i]=massiv[N-1-i];
        massiv[N-1-i]=remember; // запоминание элементов с индексом >N/2
    }

printf("\nВывод-->");

for (i = 0; i < N; i++) // вывод итоговой матрицы
    {
        printf("%d ", massiv[i]);
    }

    printf("\n");
return 0;
}