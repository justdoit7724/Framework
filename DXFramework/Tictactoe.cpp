#include "stdafx.h"
#include "Tictactoe.h"
#include "TTTRender.h"
#include "TurnController.h"

Tictactoe::Tictactoe(TTTRender* const* ppRender)
	:ppRender(ppRender)
{
	turnController = new TurnController<MoveInfo>();

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

	turnController->FinishNewTurn(MoveInfo(x, y, isO ? SpaceKind::O : SpaceKind::X));

	isO = !isO;

}

void Tictactoe::JumpBackward()
{
	MoveInfo info;
	if (turnController->JumpBack(&info))
	{
		(*ppRender)->MoveRender(info.x, info.y, SpaceKind::Null);
		board[info.y][info.x] = SpaceKind::Null;

		isO = !isO;
	}
}

void Tictactoe::JumpForward()
{
	MoveInfo info;
	if (turnController->JumpFront(&info))
	{
		(*ppRender)->MoveRender(info.x, info.y, info.kind);
		board[info.y][info.x] = info.kind;

		isO = !isO;
	}
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

bool Tictactoe::IsFull()
{
	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < 2; ++x)
		{
			if (board[y][x]!=SpaceKind::Null)
			{
				return false;
			}
		}
	}

	return true;
}

SpaceKind Tictactoe::Kind(int x, int y)
{
	return board[y][x];
}


void Tictactoe::Notify(int id, const void* data)
{
	switch (id)
	{
	case 1:
	{
		XMINT2* idx = (XMINT2*)data;

		board[idx->x][idx->y] = isO ? SpaceKind::O : SpaceKind::X;

		(*ppRender)->MoveRender(idx->x, idx->y, isO? SpaceKind::O : SpaceKind::X);

		Move(idx->x, idx->y);
	}
		break;
	default:
		break;
	}
}
