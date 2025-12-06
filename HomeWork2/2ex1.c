/*Вывести квадратную
матрицу по заданному N*/
//более универсальный метод
#include <stdio.h>
#define N 5

int main()
{
    int i,j;
    char massiv[N][N]={0};
    for(i=0;i<N;i++)
    {
        
        for(j=0;j<N;j++)
        {
            massiv[i][j]=N*i+1+j;
            printf("%d ",massiv[i][j]);
        }
    printf("\n");
    }
}
