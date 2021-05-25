#include "Player.h"
#pragma once
class AIGreedy : public Player
{
public:
	int mark;

	AIGreedy(int mark) {
		this->mark = mark;
	}

	bool isHuman() {
		return false;
	}
	
	string name() {
		return "Greedy player";
	}

	Point play(int** board) {
		Point bestMove = Point(-1,-1);
		int bestValue = 0;
		vector <Point> legalMoves = BoardHelper::getAllLegalMoves(board, mark);

		for (Point move : legalMoves) {
			int val = BoardHelper::getScore(BoardHelper::boardAfterMove(board, mark, move.x, move.y), mark);
			if (val > bestValue) {
				bestValue = val;
				bestMove = move;
			}
		}

		return bestMove;
	}
};

