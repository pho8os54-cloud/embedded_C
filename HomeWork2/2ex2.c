/*Вывести заданный массив размером N
в обратном порядке.*/

#include <stdio.h>
#define N 10

int main()
{
    char i;
    char massiv[N]={1,2,3,4,5,6,7,8,9,10};
    char new_massiv[N];
    
    /*for(i=0;i<N-1;i++)
    {
        massiv[1]=massiv[i-1];
        printf("%d\n", massiv[1]);
    }*/
    /*i = massiv[0];
    
    massiv[0]=massiv[4];
    massiv[4]= i;
    i = massiv[1];
    massiv[1]=massiv[3];
    massiv[3]= i;
    printf("%d %d %d %d %d  ", massiv[0],massiv[1],massiv[2],
                               massiv[3],massiv[4]);*/
printf("Вывод-->");
for(i=0;i<N;i++)
    {
        new_massiv[i]=massiv[N-1-i];
        printf("%d ",new_massiv[i]);
    }
    printf("\n");
    return 0;
}