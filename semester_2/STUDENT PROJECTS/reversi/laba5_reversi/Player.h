#include "Point.h"
#include <iostream>
#include <vector>
#include "BoardHelper.h"
using namespace std;
#pragma once
class Player
{

public:

	virtual bool isHuman() = 0;

	virtual string name() = 0;

	virtual Point play(int** board) = 0;
};

