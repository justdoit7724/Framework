#pragma once

enum SpaceKind
{
	Null,
	O,
	X
};

class Tictactoe
{
public:
	Tictactoe();

	void Move(int x, int y);
	bool IsFinish(bool* isOWin);
	SpaceKind Kind(int x, int y);

	bool IsO() { return isO; }

private:

	bool isO = true;
	SpaceKind board[3][3];
};

