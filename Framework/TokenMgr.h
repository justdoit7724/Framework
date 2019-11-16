#pragma once
#include "DX_info.h"
#include <vector>

class Object;
class Camera;

class TokenMgr
{
public:
	TokenMgr(const std::vector<XMFLOAT3>& firstArrange);
	~TokenMgr();

	void Move(UINT id, XMFLOAT3 to);

	void Update();
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, UINT sceneDepth)const;

private:

	std::vector<Object*> tokens;
};