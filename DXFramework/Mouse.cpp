#include "Mouse.h"

void Mouse::Update(float spf)
{
	this->spf = spf;

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

	UpdateWheel();
}

void Mouse::UpdatePt(__int64 lparam)
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

void Mouse::StartWheel(bool isUp)
{
	if (isUp)
		wheelState += wheelSpeed * spf;
	else
		wheelState -= wheelSpeed * spf;

	wheelState = fmaxf(fminf(1, wheelState), -1);
}

void Mouse::UpdateWheel()
{
	wheelState *= sqrt(fabsf(wheelState)) / 2.0f + 0.25f;
}
