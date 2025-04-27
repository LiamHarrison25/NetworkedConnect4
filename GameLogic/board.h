#pragma once

#include <iostream>
#include <vector>

class Board {
public:
	Board();
	~Board();

	int CheckWin();
	bool PlayerMove(int col, int player);
	int GetNextOpenRow(int col) const;
	int CurrentPlayer() { return playerTurn; }
	void SetBoard(std::vector<std::vector<int>> netBoard);
	const std::vector<std::vector<int>>& GetBoard() { return board; }

private:
	std::vector<std::vector<int>> board;
	int playerTurn;
	const int COLUMN = 7;
	const int ROW = 6;
};