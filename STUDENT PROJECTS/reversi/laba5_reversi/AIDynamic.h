#pragma once
#include "Player.h"
#include "Evaluator.h"

int AIDynamic_evaluate(int** board, int mark);

class AIDynamic : public Player
{
public:
	int mark, depth;

	AIDynamic(int mark, int depth) {
		this->mark = mark;
		this->depth = depth;
	}

	bool isHuman() {
		return false;
	}

	string name() {
		return "AIDynamic";
	}

	Point play(int** board) {
		return MinMax::findBestMove(board, mark, depth, AIDynamic_evaluate);
	}

	static int evalMobility(int** board, int mark);

	static int evalCorners(int** board, int mark);

	static int evalDiscDiff(int** board, int mark);
};

