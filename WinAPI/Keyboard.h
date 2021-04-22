#pragma once
#include "Singleton.h"

#define SGL_Keyboard Keyboard::Instance()

enum class KEYBOARD_STATE
{
	NONE,
	DOWN,
	PRESSING,
	UP
};

class Keyboard
{
	SET_SINGLETON(Keyboard)

private:
	KEYBOARD_STATE m_arKey[256];

public:
	
	void Init();
	void Press(const unsigned __int64 wparam);
	void Release(const unsigned __int64 wparam);
	void Update();
	KEYBOARD_STATE GetState(const char c);
};

