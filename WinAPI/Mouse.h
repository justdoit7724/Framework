#pragma once
#include "Singleton.h"

#define SGL_Mouse Mouse::Instance()

enum class MOUSE_STATE
{
	NONE,
	DOWN,
	PRESSING,
	UP
};

class Mouse
{
	SET_SINGLETON(Mouse)

public:
	void Init();
	void Click(BOOL left);
	void Release(BOOL left);
	void Update();
	void Wheel(LPARAM lparam);
	void Move(LPARAM lparam);
	MOUSE_STATE GetClickState(BOOL left);
	XMFLOAT2 GetScnPos();

private:
	MOUSE_STATE m_lState;
	MOUSE_STATE m_rState;
	float m_fWheel;
	XMFLOAT2 m_scnPos;

};

