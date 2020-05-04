#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	SphereCollider(XMFLOAT3 center, float rad);

	virtual bool IsHit(Geometrics::Ray ray, XMFLOAT3* hitPt) override;
	void SetScale(XMFLOAT3 s) override;

	void Visualize() override;
protected:
	float rad;
private:
};
