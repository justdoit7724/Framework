#pragma once
#include "DX_info.h"
#include <vector>
#include "Geometrics.h"
#include "ObserverDP.h"

class Object;
struct Frustum;

class TokenMgr : public Observer
{
public:
	TokenMgr();
	~TokenMgr();

	void Move(UINT id, XMFLOAT3 to);

	void Update(const Geometrics::Ray ray);
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, const Frustum* frustum, UINT sceneDepth)const;

	int GetPickingTokenID() { return curPickingTokenID; }

private:
	void Notify(unsigned int id, const void* data) override;

	std::vector<Object*> tokens;
	Object* redIndicator;
	Object* greenIndicator;
	int curPickingTokenID;
};