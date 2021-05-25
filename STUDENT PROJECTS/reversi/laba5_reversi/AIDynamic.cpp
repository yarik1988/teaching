#include "AIDynamic.h"

int AIDynamic_evaluate(int** board, int mark) {
	int score = BoardHelper::getScore(board, 1) + BoardHelper::getScore(board, 2);
	int em = 1, ec = 1, ed = 1;
	if (score < 14) {
		em = 200, ec = 1000, ed = 1;
	}
	else if (score < 58) {
		em = 2, ec = 1000, ed = 1;
	}
	else {
		em = 100, ec = 100, ed = 1000;
	}
	return 
		em * AIDynamic::evalMobility(board, mark) + 
		ec * AIDynamic::evalCorners(board, mark) + 
		ed * AIDynamic::evalDiscDiff(board, mark);
}

int AIDynamic::evalMobility(int** board, int mark) {
	int moves = BoardHelper::getAllLegalMoves(board, mark).size();
	int opmoves = BoardHelper::getAllLegalMoves(board, mark == 1 ? 2 : 1).size();
	return 100 * (moves - opmoves) / (moves + opmoves + 1);
}

int AIDynamic::evalCorners(int** board, int mark) {
	int mycorners = 0, opcorners = 0;
	vector<Point> corners = { Point(0,0), Point(0,7), Point(7,0), Point(7,7) };
	for (Point corner : corners) {
		if (board[corner.x][corner.y] == mark) mycorners++;
		else if (board[corner.x][corner.y] == (mark == 1 ? 2 : 1)) opcorners++;
	}
	return 100 * (mycorners - opcorners) / (mycorners + opcorners + 1);
}

int AIDynamic::evalDiscDiff(int** board, int mark) {
	int score = BoardHelper::getScore(board, mark), opscore = BoardHelper::getScore(board, mark == 1 ? 2 : 1);
	return 100 * (score - opscore) / (score + opscore);
}
