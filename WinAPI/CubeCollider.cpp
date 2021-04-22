
#include "stdafx.h"
#include "CubeCollider.h"

CubeCollider::CubeCollider(Collider* main)
	:SubCollider(main)
{
	m_extent = XMFLOAT3(0.5f, 0.5f, 0.5f);
}

void CubeCollider::Update()
{
}

bool CubeCollider::IsHit(Geom::Ray ray, XMFLOAT3* hitPt)
{
	XMFLOAT3 right = Cross(m_up, m_forward);

	if (Dot(ray.d, right) < 0)
	{
		Geom::Plane rightP(m_center + right * m_extent.x, right, m_up, XMFLOAT2(m_extent.z, m_extent.y));

		if (Geom::IntersectRayPlane(ray, rightP, hitPt))
			return true;
	}
	if (Dot(ray.d, m_up) < 0)
	{
		Geom::Plane upP(m_center + m_up * m_extent.y, m_up, -m_forward, XMFLOAT2(m_extent.x, m_extent.z));

		if (IntersectRayPlane(ray, upP, hitPt))
			return true;
	}
	if (Dot(ray.d, m_forward) < 0)
	{
		Geom::Plane forwardP(m_center + m_forward * m_extent.z, m_forward, m_up, XMFLOAT2(m_extent.x, m_extent.y));

		if (IntersectRayPlane(ray, forwardP, hitPt))
			return true;
	}
	if (Dot(ray.d, -right) < 0)
	{
		Geom::Plane leftP(m_center - right * m_extent.x, -right, m_up, XMFLOAT2(m_extent.z, m_extent.y));

		if (IntersectRayPlane(ray, leftP, hitPt))
			return true;
	}
	if (Dot(ray.d, -m_up) < 0)
	{
		Geom::Plane downP(m_center - m_up * m_extent.y, -m_up, m_forward, XMFLOAT2(m_extent.x, m_extent.z));

		if (IntersectRayPlane(ray, downP, hitPt))
			return true;
	}
	if (Dot(ray.d, -m_forward) < 0)
	{
		Geom::Plane backP(m_center - m_forward * m_extent.z, -m_forward, m_up, XMFLOAT2(m_extent.x, m_extent.y));

		if (IntersectRayPlane(ray, backP, hitPt))
			return true;
	}

	return false;
}

void CubeCollider::SetScale(XMFLOAT3 s)
{
	m_extent = s * 0.5;
}

