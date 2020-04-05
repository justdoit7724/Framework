#include "stdafx.h"
#include "TTTAI.h"
#include "Tictactoe.h"
#include "TTTRender.h"

TTTAI::TTTAI(Tictactoe*const* ppRule, TTTRender*const* ppRender)
	:ppRule(ppRule), ppRender(ppRender)
{
	
}

void TTTAI::Move()
{
	SpaceKind moveKind = (*ppRule)->IsO() ? SpaceKind::O : SpaceKind::X;

	while (true)
	{
		int x = rand() % 3;
		int y = rand() % 3;

		if ((*ppRule)->Kind(x, y) == SpaceKind::Null)
		{
			(*ppRule)->Move(x, y);
			(*ppRender)->MoveRender(x, y, moveKind);
			return;
		}
	}
}
