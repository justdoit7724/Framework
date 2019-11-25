#pragma once
#include "Object.h"

class Token : public Object
{
public:
	Token(unsigned int id, bool p1);
	Token(bool isRed);


	void Render(const XMMATRIX& parentWorld, const XMMATRIX& vp, UINT sceneDepth) const override;

	void Move(int toID, XMFLOAT3 pos);
	unsigned int ID() { return id; }
	bool IsP1()const { return isP1; }
private:
	unsigned int id;
	bool isP1;
};