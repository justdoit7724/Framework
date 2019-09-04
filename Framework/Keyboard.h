#pragma once
#include <unordered_set>
#include <Windows.h>

class Keyboard
{
private:

	static std::unordered_set<char> keys;

public:
	static void Press(const WPARAM wparam);
	static void Release(const WPARAM wparam);
	static bool IsPressing(const char c);
	static bool IsPressing(const std::string);
};

