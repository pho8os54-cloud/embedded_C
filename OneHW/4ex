/*Поменять в целом положительном числе (типа int) значение
 третьего байта на введенное пользователем число 
 (изначальное число также вводится с клавиатуры).*/
#include <stdio.h>
int main() 
{
    unsigned int P_num;
    unsigned char Rep_num;//т.к. надо заменить только 1 байт.
    printf("Введите целое положительное число : ");
    scanf("%d", &P_num);  
    printf("Введите число(0-255) для замены 3-го байта: ");
    scanf("%hhd", &Rep_num);
    unsigned int result_num = (P_num & 0xFF00FFFF) | (Rep_num << 16);// Очищаем третий байт и устанавливаем новое значение
    printf("Исходное число: %d\n", P_num);
    printf("Ответ: %d\n", result_num);
    return 0;  
}
