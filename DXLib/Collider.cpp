
#include "stdafx.h"
#include "Collider.h"
#include "CubeCollider.h"
#include "SphereCollider.h"
#include "QuadCollider.h"

SubCollider* SubCollider::Create(COLLIDER_KIND kind, Collider* main)
{
	switch (kind)
	{
	case COLLIDER_KIND::Cube:
		return new CubeCollider(main);
	case COLLIDER_KIND::Quad:
		return new QuadCollider(main);
	case COLLIDER_KIND::Sphere:
		return new SphereCollider(main);
	}

	return nullptr;
}

SubCollider::SubCollider(Collider* main)
	:m_main(main)
{
	m_center = XMFLOAT3(0, 0, 0);
	m_forward = DIR_FORWARD;
	m_up = DIR_UP;
}

void SubCollider::Translate(XMFLOAT3 pt)
{
	m_center = pt;
}

void SubCollider::SetRotate(XMFLOAT3 f, XMFLOAT3 u)
{
	m_forward = f;
	m_up = u;
}
Collider::Collider(Object* parent)
	:
	Component(CMPNT_KIND::Collider, parent),
	m_subCollider(nullptr)
{

}

Collider::~Collider()
{
	if (m_subCollider)
		delete m_subCollider;
}

void Collider::Update(DX::Graphic* graphic)
{
	if (m_subCollider)
		m_subCollider->Update();
}

void Collider::Render(DX::Graphic* graphic) const
{
}

