#pragma once
#include "Geometrics.h"

class Object;

class Token
{
public:
	Token(unsigned int id, bool p1);
	Token(bool isRed);
	~Token();

	void Move(int toID, XMFLOAT3 pos);
	bool IsPicking(const Geometrics::Ray ray);
	unsigned int ID() { return id; }
	void UpdateBound();
	void Render(const XMMATRIX& vp, XMFLOAT3 eye, unsigned int sceneDepth)const;

	bool IsP1()const { return isP1; }
	void SetEnabled(bool b);
	Object* Obj() { return obj; }
private:
	Object* obj;
	unsigned int id;
	bool isP1;
};