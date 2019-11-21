#pragma once
#include "Geometrics.h"

class Object;
class Object;
class PlaySpace;
class Tile
{
public: 
	Tile(PlaySpace* const* space, unsigned int id);
	Tile(bool isRed);
	~Tile();

	void Move(unsigned int toID);
	void RawMove(XMFLOAT3 pos);

	void UpdateBound();
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth)const;

	bool IsPicking(const Geometrics::Ray ray);
	void SetEnabled(bool e);
	unsigned int ID() { return id; }
	Object* Obj();

private:
	Object* obj;
	PlaySpace* const* space;
	unsigned int id;
};
