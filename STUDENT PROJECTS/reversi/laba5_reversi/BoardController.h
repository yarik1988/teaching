#pragma once
#include <vector>
#include <windows.h>
#include "BoardHelper.h"
#include "Point.h"
#include "Player.h"
#include "AIGreedy.h"
#include "Human.h"
#include "AIRandom.h"
#include "AIStatic.h"
#include "AIGenius.h"
#include "AIDynamic.h"
using namespace std;

class BoardController
{
	bool isFirstMove;

public:

	int** board = new int* [8];
	vector<Point> legalMoves;
	bool awaitForClick;
	int player;

	AIDynamic ai1 = AIDynamic(1, 6);
	Player& player1 = ai1;
	Human ai2 = Human(2);
	Player& player2 = ai2;

	BoardController() {
		for (int i = 0; i < 8; i++)
			board[i] = new int[8];
		resetBoard();
	}

	void resetBoard() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				board[i][j] = 0;
		board[3][4] = 1;
		board[4][3] = 1;
		board[3][3] = 2;
		board[4][4] = 2;
		player = 1;
		awaitForClick = false;
		isFirstMove = true;
		legalMoves = BoardHelper::getAllLegalMoves(board, player);
	}

	void move(int x, int y) {
		cout << name(player) << " made move" << "(" << x << "," << y << ")" << endl << endl; 
		if (!BoardHelper::canPlay(board, player, x, y)) return;
		board[x][y] = player;
		BoardHelper::fillAlldir(board, player, x, y);
		player = player == 1 ? 2 : 1;
		legalMoves = BoardHelper::getAllLegalMoves(board, player);
		awaitForClick = false;
	}

	void gameEnd() {
		int score1 = BoardHelper::getScore(board, 1), score2 = BoardHelper::getScore(board, 2);
		cout << "Game is finished" << endl;
		cout << name(1) << " : " << score1 << endl;
		cout << name(2) << " : " << score2 << endl;
		if (score1 == score2) cout << "Tie" << endl;
		cout << name(score1 > score2 ? 1 : 2) << " won" << endl;
		system("pause");
	}

	Player& getPlayer() {
		return player == 1 ? player1 : player2;
	}

	string name(int player) {
		if (player == 1) return "Black(" + player1.name() + ")";
		return "White(" + player2.name() + ")";
	}

	void manageTurn() {
		if (isFirstMove) {
			isFirstMove = false;
			return;
		}
		if (BoardHelper::isGameFinished(board)) {
			gameEnd();
			return;
		}
		if (BoardHelper::hasAnyMoves(board, player)) {
			if (getPlayer().isHuman()) {
				awaitForClick = true;
			}
			else {
				AImove();
			}
		}
		else {
			player = player == 1 ? 2 : 1;
			legalMoves = BoardHelper::getAllLegalMoves(board, player);
		}
	}

	void AImove() {
		Sleep(1000);
		Point AImove = getPlayer().play(board);
		move(AImove.x, AImove.y);
	}
};

