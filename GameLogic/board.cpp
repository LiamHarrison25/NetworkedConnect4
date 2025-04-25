#include "board.h"

Board::Board()
{
	board = std::vector<std::vector<int>>(6, std::vector<int>(7, 0));
	playerTurn = 1;
}

void Board::SetBoard(std::vector<std::vector<int>> netBoard)
{
	board = netBoard;
}

bool Board::PlayerMove(int col, int player)
{
	for (int i = 0; i < ROW; i++)
	{
		switch (board[col][i])
		{
		case 0:
			board[col][i] = player;
			return true;
		case 1:
			break;
		case 2:
			break;
		}
	}
	return false;
}

bool Board::CheckWin() // Returns true if their is a winner/loser or a draw on the board
{
	bool boardFull = true;

	for (int x = 0; x < COLUMN; x++)
	{
		for (int y = 0; y < ROW; y++)
		{
			int player = board[x][y];

			// Checks For Draw
			if (player == 0)
			{
				boardFull = false;
				continue;
			}

			// Horizontal Win
			if (x + 3 < COLUMN && player == board[x + 1][y]
				&& player == board[x + 2][y] && player == board[x + 3][y])
			{
				return true;
			}

			// Vertical Win
			if (y + 3 < ROW && player == board[x][y + 1]
				&& player == board[x][y + 2] && player == board[x][y + 3])
			{
				return true;
			}

			// Check Diagonal Down-Right
			if (x + 3 < COLUMN && y + 3 < ROW && player == board[x + 1][y + 1]
				&& player == board[x + 2][y + 2] && board[x + 3][y + 3])
			{
				return true;
			}
			
			// Check Diagonal Up-Right
			if (x + 3 < COLUMN && y - 3 >= 0 && player == board[x + 1][y - 1]
				&& player == board[x + 2][y - 2] && board[x + 3][y - 3])
			{
				return true;
			}
		}
	}

	return boardFull;
}