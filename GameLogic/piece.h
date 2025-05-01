#pragma once

#include <iostream>
#include "../game.h"

class Piece : public GameObject{
public:
	Piece();
	~Piece();

	void PlacePiece(int col, int player);
	const int GetColumn() { return piece.first; } // Returns the column
	const int GetColor() { return piece.second; } // Returns the player "color"

	void Deserialize(std::istream& is);
	void Serialize(std::ostream& os);

private:
	std::pair<int, int> piece;
};