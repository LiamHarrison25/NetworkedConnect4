#include "piece.h"

// Sets the base piece
Piece::Piece()
{
	piece = std::make_pair(0, 0);
}

Piece::~Piece()
{

}

/// <summary>
/// Sets the column of the piece and signifies
/// which player it belongs too
/// </summary>
/// <param name="col"></param>
/// <param name="player"></param>
void Piece::PlacePiece(int col, int player)
{
	piece = std::make_pair(col, player);
}

void Piece::Serialize(std::ostream& os)
{
	GameObject::Serialize(os);

	stream_write(os, piece.first);
	stream_write(os, piece.second);
}

void Piece::Deserialize(std::istream& is) 
{
    GameObject::Deserialize(is);

	stream_read(is, piece.first);

    stream_read(is, piece.second);
}
