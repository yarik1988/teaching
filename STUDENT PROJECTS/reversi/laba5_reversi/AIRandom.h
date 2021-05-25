#include "Player.h"
#include <stdlib.h>
#include <time.h>
#pragma once
class AIRandom : public Player
{
public:
	int mark;

	AIRandom(int mark) {
		this->mark = mark;
		srand(time(NULL));
	}

	bool isHuman() {
		return false;
	}

	string name() {
		return "Random player";
	}

	Point play(int** board) {
		vector <Point> legalMoves = BoardHelper::getAllLegalMoves(board, mark);
		int i = rand() % legalMoves.size();
		return legalMoves[i];
	}
};

