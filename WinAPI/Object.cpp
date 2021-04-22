
#include "stdafx.h"

#include "Object.h"
#include "Component.h"
#include "MeshFilter.h"
#include "Transform.h"

//fundamental elements
Object::Object()
	:
	m_name("object"),
	m_parent(nullptr),
	layer(LAYER_STD)
{
	AddComponent(CMPNT_KIND::Transform);
}

Object::~Object()
{
	for (auto vCmpnt : m_mComponent)
	{
		for (auto Cmpnt : vCmpnt.second)
		{
			delete Cmpnt;
		}
	}
	m_mComponent.clear();
	for (int i = 0; i < m_vChild.size(); ++i)
	{
		delete m_vChild[i];
	}
	m_vChild.clear();

}

void Object::Update(DX::Graphic* graphic)
{
	if (!enabled)
		return;

	UpdateBound();

	for (auto vCmpnt : m_mComponent)
	{
		for (auto Cmpnt : vCmpnt.second)
		{
			Cmpnt->Update(graphic);
		}
	}

	for (auto child : m_vChild)
	{
		child->Update(graphic);
	}
}

void Object::UpdateBound()
{
	MeshFilter* mesh = (MeshFilter*)GetComponent(CMPNT_KIND::MeshFilter);
	Transform* transform = (Transform*)GetComponent(CMPNT_KIND::Transform);

	if (!mesh)
		return;

	Geom::Bound meshBound;
	mesh->GetLBound(&meshBound);
	XMMATRIX world = transform->WorldMatrix();
	XMFLOAT3 wMinPt = Multiply(meshBound.minPos, world);
	XMFLOAT3 wMaxPt = Multiply(meshBound.maxPos, world);
	m_occludeBound.p = transform->GetPos();
	m_occludeBound.rad = Length(wMinPt - wMaxPt) * 0.5f;
}

Geom::Sphere Object::GetOccludeBound()
{
	return m_occludeBound;
}

int Object::Layer() const
{
	return layer;
}

void Object::SetLayer(int l)
{
	layer = l;
}

/*
void Object::Visualize()
{

	//if (IsInsideFrustum(CameraMgr::Instance()->Main()->GetFrustum()))
	//	Debugging::Instance()->Mark(bound.p, bound.rad, Colors::LightGreen);
}
*/

void Object::Render(DX::Graphic* graphic)const
{
	for (auto vCmpnt : m_mComponent)
	{
		for (auto Cmpnt : vCmpnt.second)
		{
			Cmpnt->Render(graphic);
		}
	}
	for (auto child : m_vChild)
	{
		child->Render(graphic);
	}
}

bool Object::IsInsideFrustum(const Frustum* frustum) const
{
	if (!frustum)
		return true;

	return (
		IntersectInPlaneSphere(frustum->right, m_occludeBound) &&
		IntersectInPlaneSphere(frustum->left, m_occludeBound) &&
		IntersectInPlaneSphere(frustum->top, m_occludeBound) &&
		IntersectInPlaneSphere(frustum->bottom, m_occludeBound) &&
		IntersectInPlaneSphere(frustum->front, m_occludeBound) &&
		IntersectInPlaneSphere(frustum->back, m_occludeBound));
}

void Object::AddComponent(CMPNT_KIND kind)
{
	// check component that should be only one
	if (HasComponent(kind))
	{
		switch (kind)
		{
		case CMPNT_KIND::Transform:
			return;
		}
	}

	Component* newComponent = Component::Create(kind,this);

	m_mComponent[kind].push_back(newComponent);
}

void Object::RemoveComponent(CMPNT_KIND key, int id)
{
	if (!HasComponent(key, id))
		return;

	delete m_mComponent[key][id];
	m_mComponent[key].erase(m_mComponent[key].begin() + id);
}

Component* Object::GetComponent(CMPNT_KIND key, int id)
{
	if (!HasComponent(key, id))
		return nullptr;

	return m_mComponent[key][id];
}

Component* Object::GetComponentFromParent(CMPNT_KIND key, int id)
{
	return m_parent->GetComponent(key);
}

Component* Object::GetComponentFromChildren(CMPNT_KIND key, int id)
{
	for (int i = 0; i < m_mComponent.size(); ++i)
	{
		if (m_mComponent.find(key) != m_mComponent.end())
			m_mComponent[key];
	}

	for (int i = 0; i < m_vChild.size(); ++i)
	{
		Component* curCmpnt = m_vChild[i]->GetComponentFromChildren(key);
		if (curCmpnt) return curCmpnt;
	}

	return nullptr;
}

BOOL Object::HasComponent(CMPNT_KIND key, int id)
{
	if (m_mComponent.find(key) != m_mComponent.end())
	{
		for (auto cmpnt : m_mComponent[key])
		{
			if (cmpnt->m_id == id)
				return TRUE;
		}
	}
	return FALSE;
}

void Object::AddChild(Object* obj)
{
	if (this->HasChild(obj) || obj->HasChild(this))
		return;

	m_vChild.push_back(obj);

	obj->SetParent(this);
}

void Object::RemoveChild(std::string name)
{
	for (auto it = m_vChild.begin(); it != m_vChild.end(); ++it)
	{
		if ((*it)->GetName() == name)
		{
			(*it)->SetParent(nullptr);
			m_vChild.erase(it);

			break;
		}
	}
}

void Object::RemoveChild(int index)
{
	assert(index < m_vChild.size());

	auto it = m_vChild.begin() + index;
	(*it)->SetParent(nullptr);
	m_vChild.erase(it);
}

void Object::RemoveChild(Object* obj)
{
	for (auto it = m_vChild.begin(); it != m_vChild.end(); ++it)
	{
		if (*it == obj)
		{
			(*it)->SetParent(nullptr);
			m_vChild.erase(it);
			break;
		}
	}
}


BOOL Object::HasChild(Object* obj)
{
	for (int i = 0; i < m_vChild.size(); ++i)
	{
		if (m_vChild[i]->HasChild(obj))
			return TRUE;
	}

	return FALSE;
}

void Object::SetParent(Object* parent)
{
	m_parent = parent;
}

void Object::SetName(std::string name)
{
	m_name = name;
}

std::string Object::GetName()
{
	return m_name;
}

bool Object::IsPicking(Geom::Ray ray) const
{
	return IntersectRaySphere(ray, m_occludeBound);
}