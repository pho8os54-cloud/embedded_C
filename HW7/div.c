//деление

#include "calc.h"

float div(int first_n, int second_n)
{
    if(second_n == 0) return 0;
    else return (float)first_n / second_n;
}