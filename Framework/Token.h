#pragma once
#include "Geometrics.h"

class Object;
class PlaySpace;

class Token
{
public:
	Token(PlaySpace*const* space, unsigned int id, bool p1);
	Token(bool isRed);
	~Token();

	void Move(unsigned int toID);
	void RawMove(XMFLOAT3 pos);
	bool IsPicking(const Geometrics::Ray ray);
	unsigned int ID() { return id; }
	void UpdateBound();
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth)const;

	bool IsP1() { return isP1; }
	void SetEnabled(bool b);
private:
	Object* obj;
	PlaySpace*const* space;
	unsigned int id;
	bool isP1;
};