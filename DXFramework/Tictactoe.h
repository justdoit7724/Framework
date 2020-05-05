#pragma once
#include "ObserverDP.h"
#include "TurnController.h"

enum SpaceKind
{
	Null,
	O,
	X
};

class TTTRender;

class Tictactoe : public Observer
{
public:
	Tictactoe(TTTRender* const* ppRender);

	void Move(int x, int y);
	void JumpBackward();
	void JumpForward();
	bool IsFinish(bool* isOWin);
	bool IsFull();
	SpaceKind Kind(int x, int y);

	bool IsO() { return isO; }

private:

	bool isO = true;
	SpaceKind board[3][3];

	TTTRender*const* ppRender;
	struct MoveInfo
	{
	public:
		int x, y;
		SpaceKind kind;

		MoveInfo() {}
		MoveInfo(int x, int y, SpaceKind kind)
			:x(x), y(y), kind(kind) {}
	};
	TurnController<MoveInfo>* turnController;

	

	// Inherited via Observer
	virtual void Notify(int id, const void* data) override;
};

