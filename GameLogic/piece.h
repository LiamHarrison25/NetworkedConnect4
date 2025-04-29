#pragma once

#include <iostream>
#include "../game.h"

class Piece {
public:
	Piece();
	~Piece();

	void PlacePiece(int col, int player);
	const int GetColumn() { return piece.first; }
	const int GetColor() { return piece.second; }

private:
	std::pair<int, int> piece;
};