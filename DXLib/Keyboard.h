#pragma once

namespace DX
{
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
		static void Press(const unsigned __int64 wparam);
		static void Release(const unsigned __int64 wparam);
		static KeyState GetKey(const char c);


		static void Update();
	};
}

