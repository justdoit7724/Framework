
#include "Collider.h"

Collider::Collider(XMFLOAT3 center)
	:center(center), forward(FORWARD), up(UP)
{
}

void Collider::SetRotate(XMFLOAT3 f, XMFLOAT3 u)
{
	float d = Dot(f, u);
	assert(0.0001f > abs(d));

	forward = f;
	up = u;
}
