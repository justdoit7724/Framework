#pragma once

class Keyboard
{
private:

	std::unordered_set<char> keys;

public:
	void Press(const unsigned __int64 wparam);
	void Release(const unsigned __int64 wparam);
	bool IsPressing(const char c);
};

