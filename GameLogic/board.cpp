#include "board.h"

// Sets the base board
Board::Board()
{
	board = std::vector<std::vector<int>>(7, std::vector<int>(6, 0));
}

Board::~Board()
{

}

/// <summary>
/// Clears the board back to the base while
/// clear all pieces from the pieces array.
/// </summary>
void Board::ResetGame()
{
	board = std::vector<std::vector<int>>(7, std::vector<int>(6, 0));
	pieces.clear();
}

/// <summary>
/// Refreshes the board to show the accurate
/// state of the board with the pieces it has.
/// </summary>
void Board::SetBoard()
{
	board = std::vector<std::vector<int>>(7, std::vector<int>(6, 0));

	for (int i = 0; i < pieces.size(); i++)
	{
		PlayerMove(pieces[i]);
	}
}

/// <summary>
/// Places an int based on the the piece's column
/// number (first pair), and sets the player number
/// (secod pair), in the corresponding column.
/// </summary>
/// <param name="piece"></param>
/// <returns></returns>
bool Board::PlayerMove(Piece& piece)
{
	for (int i = ROW - 1; i >= 0; i--)
	{
		switch (board[piece.GetColumn()][i])
		{
		case 0:
			board[piece.GetColumn()][i] = piece.GetColor();
			return true;
		case 1:
			break;
		case 2:
			break;
		}
	}
	return false;
}

/// <summary>
/// Checks to see if the current row is open
/// for piece placement or not. If it isn't,
/// then it returns -1.
/// </summary>
/// <param name="col"></param>
/// <returns></returns>
int Board::GetNextOpenRow(int col) const
{
	for (int row = ROW - 1; row >= 0; row--)
	{
		if (board[col][row] == 0)
			return row;
	}
	return -1;
}

// Returns the player int if there's a win, a three if it's a draw, and zero if the game isn't over
int Board::CheckWin()
{
	int boardFull = 3;

	for (int x = 0; x < COLUMN; x++)
	{
		for (int y = 0; y < ROW; y++)
		{
			int player = board[x][y];

			// Checks For Draw
			if (player == 0)
			{
				boardFull = 0;
				continue;
			}

			// Horizontal Win
			if (x + 3 < COLUMN && player == board[x + 1][y]
				&& player == board[x + 2][y] && player == board[x + 3][y])
			{
				return player;
			}

			// Vertical Win
			if (y + 3 < ROW && player == board[x][y + 1]
				&& player == board[x][y + 2] && player == board[x][y + 3])
			{
				return player;
			}

			// Check Diagonal Down-Right
			if (x + 3 < COLUMN && y + 3 < ROW && player == board[x + 1][y + 1]
				&& player == board[x + 2][y + 2] && player == board[x + 3][y + 3])
			{
				return player;
			}
			
			// Check Diagonal Up-Right
			if (x + 3 < COLUMN && y - 3 >= 0 && player == board[x + 1][y - 1]
				&& player == board[x + 2][y - 2] && player == board[x + 3][y - 3])
			{
				return player;
			}
		}
	}

	return boardFull;
}