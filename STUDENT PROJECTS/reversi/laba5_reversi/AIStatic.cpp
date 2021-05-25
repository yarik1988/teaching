#include "AIStatic.h"

int AIStatic_evaluate(int** board, int mark) {
	return 2 * AIStatic::evalMobility(board, mark) + AIStatic::evalDiscDiff(board, mark) + 1000 * AIStatic::evalCorners(board, mark);
}

int AIStatic::evalMobility(int** board, int mark) {
	int moves = BoardHelper::getAllLegalMoves(board, mark).size();
	int opmoves = BoardHelper::getAllLegalMoves(board, mark == 1 ? 2 : 1).size();
	return 100 * (moves - opmoves) / (moves + opmoves + 1);
}

int AIStatic::evalCorners(int** board, int mark) {
	int mycorners = 0, opcorners = 0;
	vector<Point> corners = { Point(0,0), Point(0,7), Point(7,0), Point(7,7) };
	for (Point corner : corners) {
		if (board[corner.x][corner.y] == mark) mycorners++;
		else if (board[corner.x][corner.y] == (mark == 1 ? 2 : 1)) opcorners++;
	}
	return 100 * (mycorners - opcorners) / (mycorners + opcorners + 1);
}

int AIStatic::evalDiscDiff(int** board, int mark) {
	int score = BoardHelper::getScore(board, mark), opscore = BoardHelper::getScore(board, mark == 1 ? 2 : 1);
	return 100 * (score - opscore) / (score + opscore);
}