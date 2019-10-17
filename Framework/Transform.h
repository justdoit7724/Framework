#pragma once
#include "Geometrics.h"

class Transform
{
public:
	XMMATRIX WorldMatrix()const;
	XMMATRIX S()const;
	XMMATRIX R()const;
	XMMATRIX T()const;

	
	XMFLOAT3 GetPos()const {return pos;}
	XMFLOAT3 GetForward()const {
		return forward;
	}
	XMFLOAT3 GetUp()const
	{
		return up;
	}
	XMFLOAT3 GetRight()const
	{
		return right;
	}
	XMFLOAT3 GetScale()const
	{
		return scale;
	}
	void SetTranslation(float x, float y, float z) {
		pos = { x,y,z };
	}
	void SetTranslation(XMFLOAT3 newPos) {
		pos = newPos;
	}
	void SetRot(XMFLOAT3 _forward)
	{
		forward = _forward;
		up = XMFLOAT3(0, 0, 0);
		right = XMFLOAT3(0, 0, 0);
	}
	void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up)
	{
#ifdef _DEBUG
		int a = 0;
#else
		assert(false);
#endif
	}
	void SetRot(XMFLOAT3 _forward, XMFLOAT3 _up, XMFLOAT3 _right) {
		forward = _forward;
		up = _up;
		right = _right;
	}
	void SetScale(float x) {
		scale = { x,x,x };
	}
	void SetScale(float x, float y, float z) {
		scale = { x,y,z };
	}
	void SetScale(XMFLOAT3 newScale) {
		scale = newScale;
	}

private:

	XMFLOAT3 pos = XMFLOAT3(0.0f,0.0f,0.0f);
	XMFLOAT3 forward = FORWARD;
	XMFLOAT3 right = RIGHT;
	XMFLOAT3 up = UP;
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f,1.0f);
};

