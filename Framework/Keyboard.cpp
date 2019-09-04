#include "Keyboard.h"

std::unordered_set<char> Keyboard::keys;

void Keyboard::Press(const WPARAM wparam)
{
	const char c = static_cast<char>(wparam);

	keys.insert(c);
}

void Keyboard::Release(const WPARAM wparam)
{
	const char c = static_cast<char>(wparam);

	auto i = keys.find(c);
	if (i  != keys.end())
	{
		keys.erase(i);
	}
}

bool Keyboard::IsPressing(const char c)
{

	return (keys.find(c) != keys.end());
}

bool Keyboard::IsPressing(const std::string s)
{
	for (const char c : s) {

		if (keys.find(c) != keys.end()) {
			return true;
		}
	}

	return false;
}
