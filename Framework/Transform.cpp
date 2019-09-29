#include "Transform.h"
#include "Camera.h"

XMMATRIX Transform::WorldMatrix()const
{
	return
		S()*
		R()*
		T();
}

XMMATRIX Transform::S()const
{
	return XMMatrixScaling(scale.x, scale.y, scale.z);
}

XMMATRIX Transform::R()const
{
	return XMMATRIX(
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		forward.x, forward.y, forward.z, 0,
		0, 0, 0, 1);;
}

XMMATRIX Transform::T()const
{
	return XMMatrixTranslation(pos.x, pos.y, pos.z);
}
