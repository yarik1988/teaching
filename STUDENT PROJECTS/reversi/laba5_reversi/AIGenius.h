#pragma once
#include "Player.h"
#include "Evaluator.h"

int AIGenius_evaluate(int** board, int mark);

class AIGenius : public Player
{
public:
	int mark, depth;

	AIGenius(int mark, int depth) {
		this->mark = mark;
		this->depth = depth;
	}

	bool isHuman() {
		return false;
	}

	string name() {
		return "AIGenius";
	}

	Point play(int** board) {
		return MinMax::findBestMove(board, mark, depth, AIGenius_evaluate);
	}

	static int evalMobility(int** board, int mark);
	
	static int evalCorners(int** board, int mark);

	static int evalDiscDiff(int** board, int mark);
};

