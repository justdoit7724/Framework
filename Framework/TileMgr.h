#pragma once
#include <vector>
#include "Geometrics.h"

class Object;
class Camera;
struct Frustum;

class TileMgr
{
public:
	TileMgr(const std::vector<XMFLOAT3>& firstArrange);
	~TileMgr();

	void Move(unsigned int id, XMFLOAT3 to);
	void Update();
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, const Frustum* frustum, unsigned int sceneDepth);

private:
	void Render(unsigned int id, const XMMATRIX& vp, XMFLOAT3 eye);
	std::vector<Object*> tiles;
};