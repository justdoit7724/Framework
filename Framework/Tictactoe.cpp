#include "stdafx.h"
#include "Tictactoe.h"

Tictactoe::Tictactoe()
{
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			board[y][x] = SpaceKind::Null;
		}
	}
}

void Tictactoe::Move(int x, int y)
{
	board[y][x] = isO ? SpaceKind::O : SpaceKind::X;

	isO = !isO;
}

bool Tictactoe::IsFinish(bool* isOWin)
{
	// horizontal
	for (int y = 0; y < 3; y++)
	{
		bool isSame = true;
		for (int x = 0; x < 2; ++x)
		{
			if (board[y][x] != board[y][x + 1])
			{
				isSame = false;
				break;
			}
		}
		if (!isSame)
			continue;
		switch (board[y][0])
		{
		case SpaceKind::O:
			*isOWin = true;
			return true;
		case SpaceKind::X:
			*isOWin = false;
			return true;
		}
	}
	// vertical
	for (int x = 0; x < 3; x++)
	{
		bool isSame = true;
		for (int y = 0; y < 2; ++y)
		{
			if (board[y][x] != board[y + 1][x])
			{
				isSame = false;
				break;
			}
		}
		if (!isSame)
			continue;
		switch (board[0][x])
		{
		case SpaceKind::O:
			*isOWin = true;
			return true;
		case SpaceKind::X:
			*isOWin = false;
			return true;
		}
	}

	// diagonal
	bool isSame = true;
	for (int i = 0; i < 2; ++i)
	{
		if (board[i][i] != board[i + 1][i + 1])
		{
			isSame = false;
			break;
		}
	}
	if (isSame)
	{
		switch (board[0][0])
		{
		case SpaceKind::O:
			*isOWin = true;
			return true;
		case SpaceKind::X:
			*isOWin = false;
			return true;
		}
	}

	isSame = true;
	for (int i = 0; i < 2; ++i)
	{
		if (board[i][2 - i] != board[i+1][1 - i])
		{
			isSame = false;
			break;
		}
	}
	if (isSame)
	{
		switch (board[0][2])
		{
		case SpaceKind::O:
			*isOWin = true;
			return true;
		case SpaceKind::X:
			*isOWin = false;
			return true;
		}
	}

	return false;
}

SpaceKind Tictactoe::Kind(int x, int y)
{
	return board[y][x];
}
