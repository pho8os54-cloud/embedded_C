/*заполнить матрицу числами от 1 до N^2 улиткой */

#include <stdio.h>

#define N 10

int main() 
{
    int massiv[N][N] = {0}; // все нули
    int row=0,column= 0; //начальная позиция -- левый верхний угол
    //направление куда идти:
    int dirs[4][2] = { {0,1},// вправо,
                       {1,0},//вниз,
                       {0,-1},//влево,
                       {-1,0} };//вверх. 
    int dir = 0;  // направление
    
    for (int num = 1; num <= N*N; num++) // проходка от 1 до N*N
    {
        massiv[row][column] = num; //запись текущего числа в массив 

        int new_r = row + dirs[dir][0];
        int new_c = column + dirs[dir][1];

        if (new_r < 0 || new_r >= N || new_c < 0 || new_c >= N || massiv[new_r][new_c] != 0) // проверка можно ли идти в след элемент массива
        {
            dir = (dir + 1) % 4;          // чтобы после 3 снова было 0 напрвление 
            new_r = row + dirs[dir][0];
            new_c = column + dirs[dir][1];
        }

        row= new_r;//шаг в новом направлении
        column = new_c;
    }

    
    for (int i = 0; i < N; i++) // вывод
    {
        for (int j = 0; j < N; j++)
        {
            printf("%3d ", massiv[i][j]);
        }
        printf("\n");
    }

    return 0;
}
