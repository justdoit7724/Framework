#pragma once
#include "DefComponent.h"

class Object;

class Component
{
public:
	static Component* Create(CMPNT_KIND kind, Object* owner);

	Component(CMPNT_KIND compKey, Object* object);
	~Component();

	virtual void Update(DX::Graphic* graphic) = 0;
	virtual void Render(DX::Graphic* graphic)const = 0;

	// identifying
	// kind of component
	const CMPNT_KIND m_key;
	// id of among same component
	const int m_id;
	// owner
	Object*const  m_object;

private:
	Component() = delete;
	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;
};
