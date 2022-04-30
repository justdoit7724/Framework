#pragma once
#include "Collider.h"

class QuadCollider :public SubCollider
{
public:
	QuadCollider(Collider* main);

	void Update() override;

	bool IsHit(Geom::Ray ray, XMFLOAT3* hitPt) override;
	void SetScale(XMFLOAT3 s) override;
private:

	XMFLOAT2 m_extent;

};
