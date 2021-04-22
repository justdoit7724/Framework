#include "stdafx.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Animator.h"
#include "Light.h"
#include "MeshFilter.h"
#include "Collider.h"

int g_id = 0;

Component* Component::Create(CMPNT_KIND kind, Object* owner)
{
	Component* newComponent = nullptr;

	switch (kind)
	{
	case CMPNT_KIND::Transform:
	{
		newComponent = new Transform(owner);
	}
	break;
	case CMPNT_KIND::Camera:
	{
		newComponent = new Camera(owner);
	}
	break;
	case CMPNT_KIND::MeshFilter:
	{
		newComponent = new MeshFilter(owner);
	}
	break;
	case CMPNT_KIND::MeshRenderer:
	{
		newComponent = new MeshRenderer(owner);
	}
	break;
	case CMPNT_KIND::Animator:
	{
		newComponent = new Animator(owner);
	}
	break;
	case CMPNT_KIND::Light:
	{
		newComponent = new Light(owner);
	}
	break;
	case CMPNT_KIND::Collider:
	{
		newComponent = new Collider(owner);
	}
	break;
	default:
	{
	}
	break;
	}

	return newComponent;
}

Component::Component(CMPNT_KIND key, Object* object)
	:
	m_key(key),
	m_object(object),
	m_id(g_id++)
{

}

Component::~Component()
{
}
