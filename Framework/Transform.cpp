#include "Transform.h"
#include "Camera.h"

XMMATRIX Transform::WorldMatrix()
{
	return
		S()*
		R()*
		T();
}

XMMATRIX Transform::S()
{
	return XMMatrixScaling(scale.x, scale.y, scale.z);
}

XMMATRIX Transform::R()
{
	return XMMATRIX(
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		forward.x, forward.y, forward.z, 0,
		0, 0, 0, 1);;
}

XMMATRIX Transform::T()
{
	return XMMatrixTranslation(pos.x, pos.y, pos.z);
}
