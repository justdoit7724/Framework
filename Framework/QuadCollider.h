#pragma once
#include "Collider.h"
#include "Network.h"

class QuadCollider :public Collider
{
public:
	QuadCollider();
	QuadCollider(XMFLOAT3 pos);

	void Visualize() override;
	bool IsHit(Ray ray, XMFLOAT3* hitPt) override;
	void SetScale(XMFLOAT3 s) override;
private:

	XMFLOAT2 extent;

};
