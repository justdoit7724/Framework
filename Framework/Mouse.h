#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "Singleton.h"

class Mouse : public Singleton<Mouse>
{
public:

	void UpdatePt(LPARAM lparam);
	void UpdateLeft(const bool press);
	void UpdateRight(const bool press);

	float X() { return pt.x; }
	float Y() { return pt.y; }
	DirectX::XMFLOAT2 LeftDragStartPt() { return leftDragStartPt;}
	DirectX::XMFLOAT2 RightDragStartPt() { return rightDragStartPt;}
	bool IsLeftDown(){ return isLeftPressed;}
	bool IsRightDown(){ return isRightPressed;}
	
private:
	DirectX::XMFLOAT2 pt= DirectX::XMFLOAT2(0,0);
	bool isLeftPressed=false;
	bool isRightPressed=false;
	DirectX::XMFLOAT2 leftDragStartPt = DirectX::XMFLOAT2(0, 0);
	DirectX::XMFLOAT2 rightDragStartPt = DirectX::XMFLOAT2(0, 0);
};

