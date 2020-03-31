#include "stdafx.h"
#include "QuadCollider.h"
#include "Debugging.h"

QuadCollider::QuadCollider()
	:Collider(XMFLOAT3(0,0,0)), extent(XMFLOAT2(0.5f,0.5f))
{
}

QuadCollider::QuadCollider(XMFLOAT3 pos)
	:Collider(pos), extent(XMFLOAT2(0.5f,0.5f))
{
}

void QuadCollider::Visualize()
{
	XMFLOAT3 right = Cross(up, forward);

	Debugging::Instance()->PtLine(center + right * extent.x + up * extent.y, center - right * extent.x + up * extent.y, Colors::LightGreen);
	Debugging::Instance()->PtLine(center + right * extent.x + up * extent.y, center + right * extent.x - up * extent.y, Colors::LightGreen);
	Debugging::Instance()->PtLine(center - right * extent.x - up * extent.y, center + right * extent.x - up * extent.y, Colors::LightGreen);
	Debugging::Instance()->PtLine(center - right * extent.x - up * extent.y, center - right * extent.x + up * extent.y, Colors::LightGreen);
}

bool QuadCollider::IsHit(Ray ray, XMFLOAT3* hitPt)
{
	return IntersectRayPlane(ray, Plane(center, forward, up, extent), hitPt);
}

void QuadCollider::SetScale(XMFLOAT3 s)
{
	extent.x = 0.5f * s.x;
	extent.y = 0.5f * s.y;
}
