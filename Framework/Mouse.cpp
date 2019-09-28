#include "Mouse.h"

void Mouse::UpdatePt(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	pt.x = p.x;
	pt.y = p.y;
}

void Mouse::UpdateLeft(const bool press)
{
	isLeftPressed = press;

	leftDragStartPt = pt;
}

void Mouse::UpdateRight(const bool press)
{
	isRightPressed = press;

	rightDragStartPt = pt;
}
