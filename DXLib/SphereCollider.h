#pragma once
#include "Collider.h"
class SphereCollider : public SubCollider
{
public:
    SphereCollider(Collider* main);

	void Update() override;

	bool IsHit(Geom::Ray ray, XMFLOAT3* hitPt)override;

	void SetScale(XMFLOAT3 s)override;

private:
	float m_rad;
};

