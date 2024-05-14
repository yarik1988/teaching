#pragma once
#include "Point.h"
#include "BoardHelper.h"
#include "MinMax.h"
#include <iostream>
using namespace std;

class Evaluator
{
public:
	virtual int evaluate(int** board, int mark) = 0;
};

