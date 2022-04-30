#include "stdafx.h"
#include "Mouse.h"

SET_SINGLETON_CPP(Mouse, Init)


void Mouse::Init()
{
	m_lState= MOUSE_STATE::NONE;
	m_rState= MOUSE_STATE::NONE;
	m_fWheel = 0;
	m_scnPos = XMFLOAT2(0, 0);
}

void Mouse::Click(BOOL left)
{
	if (left)
		m_lState = MOUSE_STATE::DOWN;
	else
		m_rState = MOUSE_STATE::DOWN;
}

void Mouse::Release(BOOL left)
{
	if (left)
		m_lState = MOUSE_STATE::UP;
	else
		m_rState = MOUSE_STATE::UP;
}

void Mouse::Update()
{
	switch (m_lState)
	{
	case MOUSE_STATE::DOWN:
		m_lState = MOUSE_STATE::PRESSING;
		break;
	case MOUSE_STATE::UP:
		m_lState = MOUSE_STATE::NONE;
		break;
	}
	switch (m_rState)
	{
	case MOUSE_STATE::DOWN:
		m_rState = MOUSE_STATE::PRESSING;
		break;
	case MOUSE_STATE::UP:
		m_rState = MOUSE_STATE::NONE;
		break;
	}
}

void Mouse::Wheel(LPARAM lparam)
{
}

void Mouse::Move(LPARAM lparam)
{
	POINTS p = MAKEPOINTS(lparam);
	m_scnPos.x = p.x;
	m_scnPos.y = p.y;
}

MOUSE_STATE Mouse::GetClickState(BOOL left)
{
	return left ? m_lState : m_rState;
}

XMFLOAT2 Mouse::GetScnPos()
{
	return m_scnPos;
}
