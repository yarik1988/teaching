#pragma once
#include "Player.h"
#include "Evaluator.h"

int AIStatic_evaluate(int** board, int mark);

class AIStatic : public Player
{

public:
	int mark, depth;

	AIStatic(int mark, int depth) {
		this->mark = mark;
		this->depth = depth;
	}

	bool isHuman() {
		return false;
	}

	string name() {
		return "AIStatic";
	}

	Point play(int** board) {
		return MinMax::findBestMove(board, mark, depth, AIStatic_evaluate);
	}

	static int evalMobility(int** board, int mark);

	static int evalCorners(int** board, int mark);

	static int evalDiscDiff(int** board, int mark);
};

