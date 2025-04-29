#include "piece.h"

Piece::Piece()
{
	piece = std::make_pair(0, 0);
}

Piece::~Piece()
{

}

void Piece::PlacePiece(int col, int player)
{
	piece = std::make_pair(col, player);
}