#include <iostream>
#include <cmath>
#include <chrono>
#include "squarerootmethods.h"
using namespace std;
typedef float (*sqrt_fun_float)(float);
typedef double (*sqrt_fun_double)(double);
int main()
{
    float input=0;
    sqrt_fun_float fn_float[]={sqrtf,sqrt1,sqrt2,sqrt3,sqrt4,sqrt5,sqrt7,sqrt15};
    sqrt_fun_double fn_double[]={sqrt6,sqrt8,sqrt9,sqrt10,sqrt13};

    for (int i=0;i<8;i++)
        std::cout<<fn_float[i](input)<<std::endl;


        /*
    for (int i=0;i<5;i++)
        std::cout<<fn_double[i](input)<<std::endl;

    std::cout<<sqrt11(input)<<std::endl;
    std::cout<<sqrt12(input)<<std::endl;
    std::cout<<sqrt14(input)<<std::endl;*/

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    double res=0;
    for (float i=1;i<1000000;i++)  res+=sqrt15(i);
    std::cout<<res;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "[ms]" << std::endl;


    begin = std::chrono::steady_clock::now();
    res=0;
    for (float i=1;i<1000000;i++)  res+=sqrtf(i);
    std::cout<<res;
    end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "[ms]" << std::endl;

    return 0;
}

