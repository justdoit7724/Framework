#pragma once
#include <vector>
#include "Geometrics.h"
#include "ObserverDP.h"

class Object;
struct Frustum;

class TileMgr : public Observer
{
public:
	TileMgr();
	~TileMgr();

	void Move(unsigned int id, XMFLOAT3 to);
	void Update(const Geometrics::Ray ray);
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, const Frustum* frustum, unsigned int sceneDepth);

	unsigned int GetCurTileID() { return curTileID; }

private:
	void Notify(unsigned int id, const void* data) override;

	void Render(unsigned int id, const XMMATRIX& vp, XMFLOAT3 eye);
	std::vector<Object*> tiles;

	unsigned int curTileID;
};