#pragma once
#include <Windows.h>
#include <unordered_map>
enum KeyState
{
	KeyState_Null,
	KeyState_Down,
	KeyState_Pressing,
	KeyState_Up
};
class Keyboard
{
private:
	
	static std::unordered_map<char, KeyState> keys;

public:
	static void Press(const WPARAM wparam);
	static void Release(const WPARAM wparam);
	static KeyState GetKey(const char c);

	
	static void Update();
};

