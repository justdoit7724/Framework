
#include "pch.h"

#include "Keyboard.h"

using namespace DX;

std::unordered_map<char, KeyState> Keyboard::keys;

void Keyboard::Press(const unsigned __int64 wparam)
{
	const char c = static_cast<char>(wparam);

	keys.insert(std::pair<char, KeyState>(c,KeyState::KeyState_Down));
}

void Keyboard::Release(const unsigned __int64 wparam)
{
	const char c = static_cast<char>(wparam);

	if (keys.find(c) != keys.end())
	{
		keys[c] = KeyState::KeyState_Up;
	}
}

KeyState Keyboard::GetKey(const char c)
{
	if (keys.find(c) == keys.end())
	{
		return KeyState::KeyState_Null;
	}

	return keys[c];
}

void Keyboard::Update()
{
	for (auto i = keys.begin(); i != keys.end();)
	{
		switch (i->second)
		{
		case KeyState::KeyState_Down:
			i->second = KeyState::KeyState_Pressing;
			return;
		case KeyState::KeyState_Up:
			keys.erase(i);
			return;
		}

		i++;
	}
}
