#pragma once
#include "Geometrics.h"

class Transform
{
public:
	XMMATRIX WorldMatrix();
	XMMATRIX S();
	XMMATRIX R();
	XMMATRIX T();

	
	XMFLOAT3 GetPos() {return pos;}
	XMFLOAT3 GetForward() {
		return forward;
	}
	void SetTranslation(float x, float y, float z) {
		pos = { x,y,z };
	}
	void SetTranslation(XMFLOAT3 newPos) {
		pos = newPos;
	}
	void SetRot(const XMFLOAT3 _forward = FORWARD, const XMFLOAT3 _up = UP) {
		forward = _forward;
		up = _up;
		right = Cross(up, forward);
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

