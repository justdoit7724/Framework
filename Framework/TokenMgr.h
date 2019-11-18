#pragma once
#include "DX_info.h"
#include <vector>
#include "Geometrics.h"

class Object;
class Camera;

class TokenMgr
{
public:
	TokenMgr(const std::vector<XMFLOAT3>& firstArrange);
	~TokenMgr();

	void Move(UINT id, XMFLOAT3 to);

	void Update(const Geometrics::Ray ray);
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, UINT sceneDepth)const;

	int GetPickingTokenID() { return curPickingTokenID; }

private:

	std::vector<Object*> tokens;
	int curPickingTokenID;
};