#include "Mouse.h"

void Mouse::Update()
{
	switch (leftState)
	{
	case MOUSE_STATE_DOWN:
		leftState = MOUSE_STATE_PRESSING;
		break;
	case MOUSE_STATE_UP:
		leftState = MOUSE_STATE_RELEASE;
	}
	switch (rightState)
	{
	case MOUSE_STATE_DOWN:
		rightState = MOUSE_STATE_PRESSING;
		break;
	case MOUSE_STATE_UP:
		rightState = MOUSE_STATE_RELEASE;
	}
}

void Mouse::UpdatePt(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	pt.x = p.x;
	pt.y = p.y;
}

void Mouse::UpdateLeft(const bool press)
{
	leftState = press ? MOUSE_STATE_DOWN : MOUSE_STATE_UP;

	leftDragStartPt = pt;
}

void Mouse::UpdateRight(const bool press)
{
	rightState = press ? MOUSE_STATE_DOWN : MOUSE_STATE_UP;

	rightDragStartPt = pt;
}
