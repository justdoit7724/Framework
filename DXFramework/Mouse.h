#pragma once
#include "Singleton.h"

enum MOUSE_STATE
{
	MOUSE_STATE_RELEASE,
	MOUSE_STATE_DOWN,
	MOUSE_STATE_PRESSING,
	MOUSE_STATE_UP
};
	

class Mouse : public Singleton<Mouse>
{
public:

	void Update(float spf);
	void UpdatePt(__int64 lparam);
	void UpdateLeft(const bool press);
	void UpdateRight(const bool press);
	void StartWheel(bool isUp);

	XMFLOAT2 Pos() { return pt; }
	XMFLOAT2 LeftDragStartPt() { return leftDragStartPt;}
	XMFLOAT2 RightDragStartPt() { return rightDragStartPt;}
	MOUSE_STATE LeftState(){ return leftState;}
	MOUSE_STATE RightState(){ return rightState;}
	float GetWheel() { return wheelState; }
	
private:
	void UpdateWheel();
	DirectX::XMFLOAT2 pt= DirectX::XMFLOAT2(0,0);
	MOUSE_STATE leftState= MOUSE_STATE_RELEASE;
	MOUSE_STATE rightState= MOUSE_STATE_RELEASE;
	DirectX::XMFLOAT2 leftDragStartPt = DirectX::XMFLOAT2(0, 0);
	DirectX::XMFLOAT2 rightDragStartPt = DirectX::XMFLOAT2(0, 0);

	float spf = 0;
	float wheelState=0;
	const float wheelSpeed = 15;
	const float wheelAtten = 8;
};

