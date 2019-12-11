#include "stdafx.h"
#include "Transform.h"
#include "Camera.h"

XMMATRIX Transform::WorldMatrix()const
{
	return XMMATRIX(
		right.x * scale.x, right.y * scale.x, right.z * scale.x, 0,
		up.x * scale.y, up.y * scale.y, up.z * scale.y, 0,
		forward.x * scale.z, forward.y * scale.z, forward.z * scale.z, 0,
		pos.x, pos.y, pos.z, 1);
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
