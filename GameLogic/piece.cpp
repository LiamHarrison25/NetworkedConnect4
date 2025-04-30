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

void Piece::Serialize(std::ostream& os)
{
	GameObject::Serialize(os);

	stream_write(os, piece.first);
	stream_write(os, piece.second);
		
}

void Piece::Deserialize(std::istream& is) 
{
    // TODO: Implement this

	GameObject::Deserialize(is);

	stream_read(is, piece.first);

    stream_read(is, piece.second);

}
