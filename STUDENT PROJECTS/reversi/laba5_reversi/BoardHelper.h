#include <vector>
#include "Point.h"
using namespace std;
#pragma once
class BoardHelper
{
public:
	static int reversedDir(int** board, int player, int x, int y, int vx, int vy) {
		int op = player == 1 ? 2 : 1;
		int i = x + vx, j = y + vy;
		int steps = 0;
		while (i >= 0 && j >= 0 && i < 8 && j < 8 && board[i][j] == op) i += vx, j += vy, steps++;
		if (i < 0 || j < 0 || i > 7 || j > 7 || board[i][j] == 0 || steps < 1) return 0;
		return steps;
	}

	static bool checkdir(int** board, int player, int x, int y, int vx, int vy) {
		return reversedDir(board, player, x, y, vx, vy) > 0;
	}

	static void filldir(int** board, int player, int x, int y, int vx, int vy) {
		if (!checkdir(board, player, x, y, vx, vy)) return;
		int op = player == 1 ? 2 : 1;
		int i = x + vx, j = y + vy;
		while (board[i][j] == op) {
			board[i][j] = player;
			i += vx;
			j += vy;
		}
	}

	static void fillAlldir(int** board, int player, int x, int y) {
		filldir(board, player, x, y, -1, 0);
		filldir(board, player, x, y, -1, 1);
		filldir(board, player, x, y, 0, 1);
		filldir(board, player, x, y, 1, 1);
		filldir(board, player, x, y, 1, 0);
		filldir(board, player, x, y, 1, -1);
		filldir(board, player, x, y, 0, -1);
		filldir(board, player, x, y, -1, -1);
	}

	static int** boardAfterMove(int** board, int player, int x, int y) {
		int** new_board = new int* [8];
		for (int i = 0; i < 8; i++) {
			new_board[i] = new int[8];
			for (int j = 0; j < 8; j++)
				new_board[i][j] = board[i][j];
		}
		new_board[x][y] = player;
		fillAlldir(new_board, player, x, y);
		return new_board;
	}

	static bool canPlay(int** board, int player, int x, int y) {
		if (x < 0 || y < 0 || x > 7 || y > 7 || board[x][y] != 0) return false;
		if (checkdir(board, player, x, y, -1, 0)) return true;
		if (checkdir(board, player, x, y, -1, 1)) return true;
		if (checkdir(board, player, x, y, 0, 1)) return true;
		if (checkdir(board, player, x, y, 1, 1)) return true;
		if (checkdir(board, player, x, y, 1, 0)) return true;
		if (checkdir(board, player, x, y, 1, -1)) return true;
		if (checkdir(board, player, x, y, 0, -1)) return true;
		if (checkdir(board, player, x, y, -1, -1)) return true;
		return false;
	}

	static vector <Point> getAllLegalMoves(int** board, int player) {
		vector<Point> result;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (canPlay(board, player, i, j)) result.push_back(Point(i, j));
		return result;
	}

	static int getScore(int** board, int player) {
		int score = 0;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (board[i][j] == player) score++;
		return score;
	}

	static bool hasAnyMoves(int** board, int player) {
		return getAllLegalMoves(board, player).size() > 0;
	}

	static bool isGameFinished(int** board) {
		if (hasAnyMoves(board, 1)) return false;
		if (hasAnyMoves(board, 2)) return false;
		return true;
	}
};

