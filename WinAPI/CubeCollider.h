#pragma once
#include "Collider.h"

class CubeCollider : public SubCollider
{
public:
	CubeCollider(Collider* main);

	void Update() override;

	bool IsHit(Geom::Ray ray, XMFLOAT3* hitPt) override;
	void SetScale(XMFLOAT3 s) override;

private:

	XMFLOAT3 m_extent;
};
