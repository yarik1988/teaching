#include "AIGenius.h"

int AIGenius_evaluate(int** board, int mark) {
	return 2 * AIGenius::evalMobility(board, mark) + 1 * AIGenius::evalDiscDiff(board, mark) + 1000 * AIGenius::evalCorners(board, mark);
}

int AIGenius::evalMobility(int** board, int mark) {
	int moves = BoardHelper::getAllLegalMoves(board, mark).size();
	int opmoves = BoardHelper::getAllLegalMoves(board, mark == 1 ? 2 : 1).size();
	return 100 * (moves - opmoves) / (moves + opmoves + 1);
}

int AIGenius::evalCorners(int** board, int mark) {
	int mycorners = 0, opcorners = 0;
	vector<Point> corners = { Point(0,0), Point(0,7), Point(7,0), Point(7,7) };
	for (Point corner : corners) {
		if (board[corner.x][corner.y] == mark) mycorners++;
		else if (board[corner.x][corner.y] == (mark == 1 ? 2 : 1)) opcorners++;
	}
	return 100 * (mycorners - opcorners) / (mycorners + opcorners + 1);
}

int AIGenius::evalDiscDiff(int** board, int mark) {
	int score = BoardHelper::getScore(board, mark), opscore = BoardHelper::getScore(board, mark == 1 ? 2 : 1);
	return 100 * (score - opscore) / (score + opscore);
}
