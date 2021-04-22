
#include "stdafx.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(Collider* main)
	:SubCollider(main)
{
	m_rad = 1;
}
void SphereCollider::Update()
{
}

bool SphereCollider::IsHit(Geom::Ray ray, XMFLOAT3* hitPt)
{
	*hitPt = XMFLOAT3(0, 0, 0);

	return IntersectRaySphere(ray, Geom::Sphere(m_center, m_rad));
}


void SphereCollider::SetScale(XMFLOAT3 s)
{
	m_rad = s.x * 0.5f;
}