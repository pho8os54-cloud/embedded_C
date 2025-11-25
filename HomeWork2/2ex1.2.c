/*Вывести квадратную
матрицу по заданному N*/
//более универсальный метод
#include <stdio.h>
#define N 5

int main()
{
    int i,j;
    char massiv[N][N]={{1,2,3,4,5},
                       {6,7,8,9,10},
                       {11,12,13,14,15},
                       {16,17,18,19,20},
                       {21,22,23,24,25}};
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            printf("%d ",massiv[i][j]);
        }
    printf("\n");
    }
return 0;
}