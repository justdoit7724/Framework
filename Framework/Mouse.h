#pragma once
#include <Windows.h>
#include <DirectXMath.h>

class Mouse
{
public:

	static void UpdatePt(LPARAM lparam);
	static void UpdateLeft(const bool press);
	static void UpdateRight(const bool press);

	static float X() { return pt.x; }
	static float Y() { return pt.y; }
	static DirectX::XMFLOAT2 LeftDragStartPt() { return leftDragStartPt;}
	static DirectX::XMFLOAT2 RightDragStartPt() { return rightDragStartPt;}
	static bool IsLeftDown(){ return isLeftPressed;}
	static bool IsRightDown(){ return isRightPressed;}
	
private:
	static DirectX::XMFLOAT2 pt;
	static bool isLeftPressed;
	static bool isRightPressed;
	static DirectX::XMFLOAT2 leftDragStartPt;
	static DirectX::XMFLOAT2 rightDragStartPt;
};

