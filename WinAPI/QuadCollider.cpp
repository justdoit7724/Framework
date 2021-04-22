
#include "stdafx.h"

#include "QuadCollider.h"

QuadCollider::QuadCollider(Collider* main)
	:SubCollider(main)
{
	m_extent = XMFLOAT2(0.5f, 0.5f);
}



void QuadCollider::Update()
{
}

bool QuadCollider::IsHit(Geom::Ray ray, XMFLOAT3* hitPt)
{
	return IntersectRayPlane(ray, Geom::Plane(m_center, m_forward, m_up, m_extent), hitPt);
}

void QuadCollider::SetScale(XMFLOAT3 s)
{
	m_extent.x = 0.5f * s.x;
	m_extent.y = 0.5f * s.y;
}
