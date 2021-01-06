#include <iostream>
#include <cmath>
#include "squarerootmethods.h"
using namespace std;
typedef float (*sqrt_fun_float)(float);
typedef double (*sqrt_fun_double)(double);
int main()
{
    float input=100;
    sqrt_fun_float fn_float[]={sqrtf,sqrt1,sqrt2,sqrt3,sqrt4,sqrt5,sqrt7};
    sqrt_fun_double fn_double[]={sqrt6,sqrt8,sqrt9,sqrt10,sqrt13};

    for (int i=0;i<7;i++)
        std::cout<<fn_float[i](input)<<std::endl;

    for (int i=0;i<5;i++)
        std::cout<<fn_double[i](input)<<std::endl;

    std::cout<<sqrt11(input)<<std::endl;
    std::cout<<sqrt12(input)<<std::endl;
    std::cout<<sqrt14(input)<<std::endl;
    return 0;
}

