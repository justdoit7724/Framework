#include "stdafx.h"
#include "Keyboard.h"

SET_SINGLETON_CPP(Keyboard, Init)

void Keyboard::Init()
{
	for (int i = 0; i < 256; ++i)
	{
		m_arKey[i] = KEYBOARD_STATE::NONE;
	}
}

void Keyboard::Press(const unsigned __int64 wparam)
{
	const char c = static_cast<char>(wparam);

	m_arKey[c] = KEYBOARD_STATE::DOWN;
}

void Keyboard::Release(const unsigned __int64 wparam)
{
	const char c = static_cast<char>(wparam);

	m_arKey[c] = KEYBOARD_STATE::UP;
}

void Keyboard::Update()
{
	for (int i = 0; i < 256; ++i)
	{
		switch (m_arKey[i])
		{
		case KEYBOARD_STATE::DOWN:
			m_arKey[i] = KEYBOARD_STATE::PRESSING;
			break;
		case KEYBOARD_STATE::UP:
			m_arKey[i] = KEYBOARD_STATE::NONE;
			break;
		}
	}
}

KEYBOARD_STATE Keyboard::GetState(const char c)
{
	assert(c < 256);

	return m_arKey[c];
}