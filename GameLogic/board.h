#pragma once

#include <iostream>
#include <vector>
#include "piece.h"

class Board {
public:
	Board();
	~Board();

	int CheckWin();
	bool PlayerMove(Piece& piece);
	void AddPiece(Piece& piece) { pieces.push_back(piece); } // Adds a piece to the pieces array
	int GetNextOpenRow(int col) const;
	void SetBoard();
	const std::vector<std::vector<int>>& GetBoard() { return board; } // Returns the board's current state
	void ResetGame();

private:
	std::vector<std::vector<int>> board;
	std::vector<Piece> pieces;
	const int COLUMN = 7;
	const int ROW = 6;
};