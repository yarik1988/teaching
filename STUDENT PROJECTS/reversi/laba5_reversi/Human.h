#include "Player.h"
#pragma once
class Human : public Player 
{
public:
	int mark;

	Human(int mark) {
		this->mark = mark;
	}

	bool isHuman() {
		return true;
	}

	string name() {
		return "Player";
	}

	Point play(int** board) {
		return Point(0, 0);
	}
};

