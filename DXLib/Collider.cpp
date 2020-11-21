
#include "pch.h"
#include "Collider.h"

using namespace DX;

Collider::Collider(XMFLOAT3 center)
	:center(center), forward(FORWARD), up(UP)
{
}

void Collider::SetRotate(XMFLOAT3 f, XMFLOAT3 u)
{
	forward = f;
	up = u;
}
