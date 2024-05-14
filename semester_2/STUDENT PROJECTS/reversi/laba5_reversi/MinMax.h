#pragma once
#include "Evaluator.h"
#include <iostream>
using namespace std;

class MinMax
{
public:

	static Point findBestMove(int** board, int mark, int depth, int (*evaluate)(int**, int)) {
		Point bestMove = Point(0, 0);
		int value = INT32_MIN;
		int nodes = 0;
		for (Point move : BoardHelper::getAllLegalMoves(board, mark)) {
			int** new_board = BoardHelper::boardAfterMove(board, mark, move.x, move.y);
			int childValue = minimax(new_board, mark, depth-1, false, INT32_MIN, INT32_MAX, evaluate, nodes);
			if (childValue > value) {
				bestMove = move;
				value = childValue;
			}
		}
		cout << "Nodes explored: " << nodes << endl;
		return bestMove;
	}

	static int minimax(int** board, int mark, int depth, bool max, int alpha, int beta, int (*evaluate)(int**, int), int &nodes) {
		nodes++;
		if (depth <= 0 || BoardHelper::isGameFinished(board)) {
			return evaluate(board, mark);
		}
		int curmark = max ? mark : (mark == 1 ? 2 : 1);
		if (!BoardHelper::hasAnyMoves(board, curmark)) {
			return minimax(board, mark, depth - 1, !max, alpha, beta, evaluate, nodes);
		}
		int score = max ? INT32_MIN : INT32_MAX;
		for (Point move : BoardHelper::getAllLegalMoves(board, curmark)) {
			int** new_board = BoardHelper::boardAfterMove(board, curmark, move.x, move.y);
			int childValue = minimax(new_board, mark, depth - 1, !max, alpha, beta, evaluate, nodes);
			if (max) {
				if (childValue > score) score = childValue;
				if (score > alpha) alpha = score;
			}
			else {
				if (childValue < score) score = childValue;
				if (score < beta) beta = score;
			}
			if (beta <= alpha) break;
		}
		return score;
	}
};

