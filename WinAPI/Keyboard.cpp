#include "stdafx.h"
#include "Keyboard.h"

void Keyboard::Press(const unsigned __int64 wparam)
{
	const char c = static_cast<char>(wparam);

	if (keys.find(c) == keys.end())
	{
		keys.insert(c);
	}
}

void Keyboard::Release(const unsigned __int64 wparam)
{
	const char c = static_cast<char>(wparam);

	if (keys.find(c) != keys.end())
	{
		keys.erase(c);
	}
}

bool Keyboard::IsPressing(const char c)
{
	return (keys.find(c) != keys.end());
}