
#include "stdafx.h"

#include "Transform.h"
#include "Math.h"
#include "Object.h"

Transform::Transform(Object* owner)
	:
	Component(CMPNT_KIND::Transform, owner),
	m_pos(0, 0, 0), m_right(DIR_RIGHT), m_up(DIR_UP), m_forward(DIR_FORWARD), m_scale(1, 1, 1)
{
}

XMMATRIX MakeWorldMatrix(XMFLOAT3 pos, XMFLOAT3 forward, XMFLOAT3 up, XMFLOAT3 right, XMFLOAT3 scale)
{
	return XMMATRIX(
		right.x * scale.x, right.y * scale.x, right.z * scale.x, 0,
		up.x * scale.y, up.y * scale.y, up.z * scale.y, 0,
		forward.x * scale.z, forward.y * scale.z, forward.z * scale.z, 0,
		pos.x, pos.y, pos.z, 1);
}

XMMATRIX Transform::WorldMatrix()const
{
	return MakeWorldMatrix(m_pos, m_forward, m_up, m_right, m_scale);
}

XMMATRIX Transform::S()const
{
	return XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
}

XMMATRIX Transform::R()const
{
	return XMMATRIX(
		m_right.x, m_right.y, m_right.z, 0,
		m_up.x, m_up.y, m_up.z, 0,
		m_forward.x, m_forward.y, m_forward.z, 0,
		0, 0, 0, 1);
}

XMMATRIX Transform::T()const
{
	return XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void Transform::SetRot(XMFLOAT3 _forward)
{
	m_forward = _forward;
	XMFLOAT3 tempUP = (m_forward == DIR_UP) ? -DIR_FORWARD : DIR_UP;
	m_right = Cross(tempUP, m_forward);
	m_up = Cross(m_forward, m_right);
}

void Transform::SetRot(XMFLOAT3 _forward, XMFLOAT3 _up)
{
	m_forward = _forward;
	m_up = _up;

	float l1 = Length(m_forward);
	float l2 = Length(m_up);
	if (l1 < 0.9f || l1 > 1.1f || l2 < 0.9f || l2 > 1.1f)
		int a = 0;
	m_right = Cross(_up, _forward);
}

void Transform::SetRot(XMFLOAT3 _forward, XMFLOAT3 _up, XMFLOAT3 _right)
{
	m_forward = _forward;
	m_up = _up;
	m_right = _right;
}

void Transform::Rotate(XMFLOAT3 axis, float rad)
{
	//  linear algebra
	//  calculated based from zero (local space)
	// apply formula for each default row of matrix

	float cosTheta = cos(rad);
	float sinTheta = sin(rad);

	XMMATRIX rotMat(
		(1.0f - cosTheta) * axis.x * axis.x + cosTheta, (1.0f - cosTheta) * axis.x * axis.y - sinTheta * axis.z, (1.0f - cosTheta) * axis.x * axis.z + sinTheta * axis.y, 0,
		(1.0f - cosTheta) * axis.x * axis.y + sinTheta * axis.z, (1.0f - cosTheta) * axis.y * axis.y + cosTheta, (1.0f - cosTheta) * axis.y * axis.z - sinTheta * axis.x, 0,
		(1.0f - cosTheta) * axis.x * axis.z - sinTheta * axis.y, (1.0f - cosTheta) * axis.y * axis.z + sinTheta * axis.x, (1.0f - cosTheta) * axis.z * axis.z + cosTheta, 0,
		0, 0, 0, 1);


	m_forward = MultiplyDir(m_forward, rotMat);
	m_up = MultiplyDir(m_up, rotMat);
	m_right = Cross(m_up, m_forward);
}

void Transform::Update(DX::Graphic* graphic)
{
	Transform* parentTF = (Transform*)m_object->GetComponentFromParent(CMPNT_KIND::Transform);
	XMMATRIX parentTransport = parentTF->T();
	XMMATRIX parentRotate = parentTF->R();
	XMMATRIX parentScale = parentTF->S();

	XMFLOAT3 finalPos = Multiply(m_pos, parentTransport);
	XMFLOAT3 finalForward = MultiplyDir(m_forward, parentRotate);
	XMFLOAT3 finalRight = MultiplyDir(m_right, parentRotate);
	XMFLOAT3 finalUp = MultiplyDir(m_up, parentRotate);
	XMFLOAT3 finalScale = Multiply(m_scale, parentScale);

	m_world = MakeWorldMatrix(finalPos, finalForward, finalUp, finalRight, finalScale);
}

void Transform::Render(DX::Graphic* graphic)const
{
	std::vector<const void*> datas;
	datas.push_back(&m_world);
	graphic->SetPipelineData(PIP_REG::TRANSFORM_WORLD, datas);
}

